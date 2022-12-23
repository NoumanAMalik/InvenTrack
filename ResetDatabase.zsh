#!/bin/zsh
# A script to quickly make Inventory.db from its 2 SQL files
rm Inventory.db
sqlite3 Inventory.db < MakeInventory.sql
sqlite3 Inventory.db < TestDataInventory.sql