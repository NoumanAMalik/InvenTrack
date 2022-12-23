-- Starting Tables: 
-- Product(Id, UPC, Name, Description, Price)
-- Inventory(Id, ProductId, Quantity)
PRAGMA foreign_keys = ON;
BEGIN TRANSACTION;
CREATE TABLE Product(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
    UPC INTEGER NOT NULL,
    Name TEXT NOT NULL,
    Description TEXT,
    Price FLOAT NOT NULL
);
CREATE TABLE Inventory(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
    ProductId INTEGER NOT NULL,
    Quantity INTEGER NOT NULL,
    CONSTRAINT FK_Inventory_ProductId_2_Product_Id FOREIGN KEY(ProductId) REFERENCES Product(Id)
);
COMMIT;