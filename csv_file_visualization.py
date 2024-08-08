import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Read and preprocess the data
df = pd.read_csv('hikaru_games_game_info.csv').drop(columns=[
    'game_id', 'site', 'round', 'white_title', 'result', 'black_title', 
    'ply_count', 'date_created', 'file_name', 'variant', 'utc_date', 'utc_time'
])

# Filter for relevant games
filtered_won_games = df[(df['event'] == 'Live Chess') & (df['time_control'] == '180') & 
                        (((df['white'] == 'Hikaru') & (df['winner'] == 'Hikaru')) | 
                         ((df['black'] == 'Hikaru') & (df['winner'] == 'Hikaru')))
                       ].drop(columns=['event'])

# Identify streaks
filtered_won_games['diff'] = filtered_won_games['game_order'].diff()
filtered_won_games['streak_group'] = (filtered_won_games['diff'] != 1).cumsum()

# Calculate streak lengths
streaks = filtered_won_games.groupby('streak_group').agg(
    streak_length=('game_order', 'size')
).reset_index()

# Filter out streaks with fewer than 10 games
streaks = streaks[streaks['streak_length'] >= 10]

# Print top streaks (Optional: Only if you want to see the top streaks of 10 or more)
top_streaks = streaks.sort_values(by='streak_length', ascending=False)
print(top_streaks)

# Plot histogram of streak lengths
plt.figure(figsize=(10, 6))
sns.histplot(streaks['streak_length'], bins=range(10, streaks['streak_length'].max() + 1), kde=False)
plt.title('Histogram of Streak Lengths (10 or more games)')
plt.xlabel('Streak Length')
plt.ylabel('Frequency')
plt.grid(False)
plt.show()
