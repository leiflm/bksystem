CREATE TABLE "bill_dates" (
   "bill_id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL,
   "timestamp" DATETIME NOT NULL
);
INSERT INTO bill_dates (timestamp) VALUES ("0000-01-01 00:00:00");
CREATE TABLE "previous_fav_products" (
   "placement" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL UNIQUE,
   "EAN" INTEGER
);
CREATE TABLE "products" (
   "EAN" INTEGER PRIMARY KEY NOT NULL  UNIQUE  check(typeof("EAN") = 'integer'),
   "name" VARCHAR NOT NULL ,
   "price" FLOAT NOT NULL  DEFAULT 0.00,
   "image" BLOB,
   "status" DEFAULT 0
);
CREATE TABLE "previous_fav_users" (
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

CREATE VIEW "date_last_bill" AS
   SELECT MAX(timestamp) AS "date"
   FROM bill_dates;

CREATE VIEW "date_second_last_bill" AS
   SELECT MAX(timestamp) AS "date"
   FROM bill_dates, date_last_bill
   WHERE timestamp < date_last_bill.date;

CREATE VIEW "previous_account_list" AS
SELECT userid, lastname, firstname, EAN, name, price, count(product) AS count, round(sum(round(price,2)),2) AS "sum" 
   FROM
      (SELECT uid, lastname, firstname 
      FROM user_accounts)
   INNER JOIN
      (
         (SELECT EAN,name FROM products) 
      INNER JOIN 
        (SELECT userid, product, price,timestamp 
         FROM sales,date_last_bill, date_second_last_bill 
         WHERE sales.timestamp > date_second_last_bill.date 
         AND sales.timestamp <= date_last_bill.date) 
      ON EAN = product
      )
   ON userid = uid
   GROUP BY userid, EAN, price
   ORDER BY lastname, firstname, EAN;

CREATE VIEW "previous_account_balance" AS
   SELECT userid, firstname, lastname, round(sum(round(price,2)),2) AS "sum"
   FROM
      (
         (SELECT uid,firstname,lastname
         FROM user_accounts)
      INNER JOIN
         (SELECT userid, product, price,timestamp
         FROM sales,date_last_bill,date_second_last_bill
         WHERE sales.timestamp > date_second_last_bill.date 
         AND sales.timestamp <= date_last_bill.date)
      ON uid =userid)
      GROUP BY uid
      ORDER BY lastname,firstname;

CREATE VIEW "previous_consumption" AS
SELECT EAN,name,price, count(userid) AS "count", round(sum(round(price,2)),2) AS "sum"
   FROM
      (
         (SELECT EAN,name
         FROM products)
      INNER JOIN
         (SELECT userid, product, price,timestamp
         FROM sales, date_last_bill,date_second_last_bill
         WHERE sales.timestamp > date_second_last_bill.date 
         AND sales.timestamp <= date_last_bill.date)
      ON EAN = product)
      GROUP BY EAN, price
      ORDER BY name;

CREATE VIEW "current_account_list" AS
SELECT userid, lastname, firstname, EAN, name, price, count(product) AS count, round(sum(round(price,2)),2) AS "sum" 
   FROM
      (SELECT uid, lastname, firstname 
      FROM user_accounts)
   INNER JOIN
      (
         (SELECT EAN,name FROM products) 
      INNER JOIN 
        (SELECT userid, product, price,timestamp 
         FROM sales,date_last_bill, date_second_last_bill 
         WHERE sales.timestamp >= date_last_bill.date) 
      ON EAN = product
      )
   ON userid = uid
   GROUP BY userid, EAN, price
   ORDER BY lastname,firstname,name;

CREATE VIEW "current_account_balance" AS
   SELECT userid, firstname, lastname, round(sum(round(price,2)),2) AS "sum"
   FROM
      (
         (SELECT uid,lastname,firstname
         FROM user_accounts)
      INNER JOIN
         (SELECT userid, product, price,timestamp
         FROM sales,date_last_bill
         WHERE  timestamp > date)
      ON uid =userid)
      GROUP BY uid
      ORDER BY lastname,firstname;

CREATE VIEW "current_consumption" AS
SELECT EAN,name,price, count(EAN) AS "count", round(sum(round(price,2)),2) AS "sum"
   FROM
      (
         (SELECT EAN,name
         FROM products )
      INNER JOIN
         (SELECT userid, product, price,timestamp
         FROM sales, date_last_bill
         WHERE  timestamp > date)
      ON EAN = product)
      GROUP BY EAN,price
      ORDER BY name;

CREATE VIEW "current_fav_products" AS
   SELECT EAN, name, image,num
   FROM products,
      (SELECT product, count(product) AS num
      FROM
         (SELECT product, timestamp
         FROM sales,date_last_bill
         WHERE timestamp > date_last_bill.date)
      GROUP BY product
      ORDER BY num desc)
   WHERE EAN = product;

CREATE VIEW "current_fav_users" AS
   SELECT uid,firstname,lastname,status,image,sum
   FROM user_accounts,
      (SELECT userid, round(sum(round(price,2)),2) AS sum
      FROM
         (SELECT userid, product, price,timestamp
         FROM sales,date_last_bill
         WHERE  timestamp>date_last_bill.date)
      GROUP BY userid
      ORDER BY sum desc)
   WHERE uid=userid;

CREATE TRIGGER "make_bill"
   BEFORE INSERT ON "bill_dates"
   BEGIN 
      SELECT CASE
         WHEN NEW.timestamp < (SELECT MAX(timestamp) FROM bill_dates) 
         THEN RAISE(ABORT, "There is a newer bill date in the table.Try a newer date!")
         WHEN NEW.timestamp > (SELECT datetime('now','localtime'))
          THEN RAISE(ABORT, "Date is in the future, Try an older date!")
      END;
   END;

CREATE TRIGGER "reload_fav"
   AFTER INSERT ON "bill_dates"
   BEGIN
      DELETE FROM previous_fav_products;
      INSERT INTO previous_fav_products (EAN) SELECT EAN FROM current_fav_products;
      DELETE FROM previous_fav_users;
      INSERT INTO previous_fav_users (uid) SELECT uid FROM current_fav_users;
   END;

CREATE TRIGGER "rewind_reload_fav"
   AFTER DELETE ON "bill_dates"
   BEGIN
      DELETE FROM previous_fav_products;
      INSERT INTO previous_fav_products (EAN) SELECT EAN FROM current_fav_products;
      DELETE FROM previous_fav_users;
      INSERT INTO previous_fav_users (uid) SELECT uid FROM current_fav_users;
   END;

CREATE TRIGGER "rewind_seq_bill_dates" AFTER DELETE ON "bill_dates"
   BEGIN
      UPDATE sqlite_sequence SET seq=(SELECT MAX(bill_id) FROM bill_dates) WHERE name = "bill_dates";
   END;

CREATE TRIGGER "rewind_seq_products"
   AFTER DELETE ON "previous_fav_products"
   BEGIN
      UPDATE sqlite_sequence SET seq=(SELECT MAX(placement) FROM previous_fav_products) WHERE name = "previous_fav_products";
   END;

CREATE TRIGGER "rewind_seq_user" AFTER DELETE ON "previous_fav_users"
   BEGIN
      UPDATE sqlite_sequence SET seq=(SELECT MAX(placement) FROM previous_fav_users) WHERE name = "previous_fav_users";
   END;

CREATE TRIGGER "check_delete_product"
   BEFORE DELETE ON "products"
   FOR EACH ROW
   BEGIN
      SELECT CASE
         WHEN OLD.EAN = (SELECT EAN FROM current_consumption)
         THEN RAISE(ABORT, "Some unpaid products left in current_comsumption. Please make a new bill first.")
      END;
      DELETE FROM sales WHERE OLD.EAN = sales.product;
      DELETE FROM previous_fav_products;
      INSERT INTO previous_fav_products (EAN) SELECT EAN FROM current_fav_products;
   END;

CREATE TRIGGER "check_delete_user"
   BEFORE DELETE ON "user_accounts"
   FOR EACH ROW
   BEGIN
      SELECT CASE
         WHEN OLD.uid = (SELECT userid FROM current_account_list)
         THEN RAISE(ABORT, "Some unpaid products left in current_account_balance. Please make a new bill first.")
      END;
      DELETE FROM sales WHERE OLD.uid = sales.userid;
      DELETE FROM previous_fav_users;
      INSERT INTO previous_fav_users (uid) SELECT uid FROM current_fav_users;
   END;
