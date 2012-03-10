CREATE TABLE "bill_dates" (
   "bill_id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL,
   "timestamp" DATETIME NOT NULL
);
CREATE TABLE "current_bill" (
   "userid" INTEGER,
   "firstname" VARCHAR,
   "lastname" VARCHAR,
   "sum" FLOAT
);
CREATE TABLE "current_consumption" (
  "product" INTEGER,
  "name" VARCHAR,
  "no" INTEGER,
  "sum" FLOAT
);
CREATE TABLE "favorite_products" (
   "placement" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL UNIQUE,
   "EAN" INTEGER
);
CREATE TABLE "products" (
   "EAN" INTEGER PRIMARY KEY NOT NULL  UNIQUE  check(typeof("EAN") = 'integer'),
   "name" VARCHAR NOT NULL ,
   "price" FLOAT NOT NULL  DEFAULT 0.00,
   "image" BLOB
);
CREATE TABLE "recent_user_accounts" (
   "placement" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE,
   "uid" INTEGER
);
CREATE TABLE "sales" (
   "userid" INTEGER NOT NULL  check(typeof("userid") = 'integer'),
   "product" INTEGER NOT NULL  check(typeof("product") = 'integer'),
   "price" FLOAT,
   "timestamp" DATETIME NOT NULL,
   FOREIGN KEY(userid) REFERENCES user_accounts(uid),
   FOREIGN KEY(product) REFERENCES products(EAN)
);
CREATE TABLE "user_accounts" (
   "uid" INTEGER PRIMARY KEY  NOT NULL  UNIQUE  check(typeof("uid") = 'integer'),
   "firstname" VARCHAR NOT NULL ,
   "lastname" VARCHAR NOT NULL ,
   "status" INTEGER DEFAULT 0,
   "image" BLOB
);
CREATE VIEW "bill" AS
   SELECT userid, firstname, lastname, round(sum(round(price,2)),2) AS "sum"
   FROM
      (
         (SELECT uid,firstname,lastname
         FROM user_accounts)
      INNER JOIN
         (SELECT userid, product, price,timestamp
         FROM sales,date_last_bill
         WHERE  timestamp > date)
      ON uid =userid)
      GROUP BY uid
      ORDER BY lastname,firstname;

CREATE VIEW "consumption" AS
SELECT EAN,name, count(userid) AS "No", round(sum(round(price,2)),2) AS "sum"
   FROM
      (
         (SELECT EAN,name
         FROM products )
      INNER JOIN
         (SELECT userid, product, price,timestamp
         FROM sales, date_last_bill
         WHERE  timestamp > date)
      ON EAN = product)
      GROUP BY EAN
      ORDER BY name;

CREATE VIEW "date_last_bill" AS
   SELECT MAX(timestamp) AS "date"
   FROM bill_dates;

CREATE VIEW "date_second_last_bill" AS
   SELECT MAX(timestamp) AS "date"
   FROM bill_dates, date_last_bill
   WHERE timestamp < date_last_bill.date;

CREATE VIEW "fav_products" AS
   SELECT EAN, name, image,num
   FROM products,
      (SELECT product, count(product) AS num
      FROM
         (SELECT product, timestamp
         FROM sales,date_last_bill,date_second_last_bill
         WHERE timestamp > date_second_last_bill.date AND timestamp < date_last_bill.date)
      GROUP BY product
      ORDER BY num desc)
   WHERE EAN = product;

CREATE VIEW "recent_users" AS
   SELECT uid,firstname,lastname,status,image,sum
   FROM user_accounts,
      (SELECT userid, round(sum(round(price,2)),2) AS sum
      FROM
         (SELECT userid, product, price,timestamp
         FROM sales,date_last_bill,date_second_last_bill
         WHERE timestamp>date_second_last_bill.date AND timestamp<date_last_bill.date)
      GROUP BY userid
      ORDER BY sum desc)
   WHERE uid=userid;

CREATE TRIGGER "make_bill"
   BEFORE INSERT ON "bill_dates"
   BEGIN
      SELECT CASE
         WHEN NEW.timestamp < (SELECT MAX(timestamp) FROM bill_dates)
         THEN RAISE(ABORT, "There is a newer bill date in the table. delete it or try a newer date!")
      END;
      DELETE FROM current_bill;
      INSERT INTO current_bill SELECT userid, firstname, lastname, sum FROM bill;
      DELETE FROM current_consumption;
      INSERT INTO current_consumption SELECT EAN, name, no, sum FROM consumption;
   END;

CREATE TRIGGER "make_bill_reload_fav"
   AFTER INSERT ON "bill_dates"
   BEGIN
      DELETE FROM favorite_products;
      INSERT INTO favorite_products (EAN) SELECT EAN FROM fav_products;
      DELETE FROM recent_user_accounts;
      INSERT INTO recent_user_accounts (uid) SELECT uid FROM recent_users;
   END;

CREATE TRIGGER "product_delete_check"
   BEFORE DELETE ON "products" FOR EACH ROW
   BEGIN
      SELECT CASE
         WHEN OLD.EAN = (SELECT EAN FROM consumption)
         THEN RAISE(ABORT, "Some unpaid products left in bill. Please make a new bill first.")
      END;
      DELETE FROM sales WHERE OLD.EAN = sales.product;
      DELETE FROM favorite_products;
      INSERT INTO favorite_products (EAN) SELECT EAN FROM fav_products;
   END;

CREATE TRIGGER "rewind_make_bill"
   AFTER DELETE ON "bill_dates"
   BEGIN
      DELETE FROM favorite_products;
      INSERT INTO favorite_products (EAN) SELECT EAN FROM fav_products;
      DELETE FROM recent_user_accounts;
      INSERT INTO recent_user_accounts (uid) SELECT uid FROM recent_users;
   END;

CREATE TRIGGER "rewind_seq_bill_dates" AFTER DELETE ON "bill_dates"
   BEGIN
      UPDATE sqlite_sequence SET seq=(SELECT MAX(bill_id) FROM bill_dates) WHERE name = "bill_dates";
   END;

CREATE TRIGGER "rewind_seq_products"
   AFTER DELETE ON "favorite_products"
   BEGIN
      UPDATE sqlite_sequence SET seq=0 WHERE name = "favorite_products";
   END;

CREATE TRIGGER "rewind_seq_user" AFTER DELETE ON "recent_user_accounts"
   BEGIN
      UPDATE sqlite_sequence SET seq=(SELECT MAX(placement) FROM recent_user_accounts) WHERE name = "recent_user_accounts";
   END;

CREATE TRIGGER "user_delete_check"
   BEFORE DELETE ON "user_accounts"
   FOR EACH ROW
   BEGIN
      SELECT CASE
         WHEN OLD.uid = (SELECT userid FROM bill)
         THEN RAISE(ABORT, "Some unpaid products left in bill. Please make a new bill first.")
      END;
      DELETE FROM sales WHERE OLD.uid = sales.userid;
      DELETE FROM recent_user_accounts;
      INSERT INTO recent_user_accounts (uid) SELECT uid FROM recent_users;
   END;
