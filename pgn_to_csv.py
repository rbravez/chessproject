from converter.pgn_data import PGNData
import os

# Ensure the path to hikaru_games.pgn is correct
pgn_file = r"C:\Users\AMON\Documents\ChessDSProject\Games\magnuscarlsen_games.pgn"
pgn_data = PGNData(pgn_file)

# Convert PGN to CSV
pgn_data.export(moves_required=False)
