CREATE TABLE "favorite_products" ("placement" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , "EAN" INTEGER);
CREATE TABLE "products" ("EAN" INTEGER PRIMARY KEY  NOT NULL  UNIQUE  check(typeof("EAN") = 'integer') , "name" VARCHAR NOT NULL , "price" FLOAT NOT NULL  DEFAULT 0.00, "image" BLOB);
CREATE TABLE "recent_user_accounts" ("placement" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , "uid" INTEGER);
CREATE TABLE "sales" ("userid" INTEGER NOT NULL  check(typeof("userid") = 'integer') , "product" INTEGER NOT NULL  check(typeof("product") = 'integer') , "price" FLOAT, "timestamp" DATETIME NOT NULL , FOREIGN KEY(userid) REFERENCES user_accounts(uid),FOREIGN KEY(product) REFERENCES products(EAN));
CREATE TABLE "user_accounts" ("uid" INTEGER PRIMARY KEY  NOT NULL  UNIQUE  check(typeof("uid") = 'integer') , "firstname" VARCHAR NOT NULL , "lastname" VARCHAR NOT NULL , "status" INTEGER DEFAULT 0, "image" BLOB);
