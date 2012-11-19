CREATE TABLE 'bills' (
   'id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL,
   'created_at' DATETIME DEFAULT (datetime('now','localtime')),
   'updated_at' DATETIME DEFAULT (datetime('now','localtime'))
);
INSERT INTO bills (created_at,updated_at) VALUES ('0000-01-01 00:00:00', '0000-01-01 00:00:00');

CREATE TABLE 'products' (
   'id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL check(typeof(id) = 'integer'),
   'ean' INTEGER,
   'name' VARCHAR NOT NULL ,
   'price' FLOAT DEFAULT 0.00,
   'image' BLOB,
   'status' INTEGER DEFAULT 0,
   'created_at' DATETIME DEFAULT (datetime('now','localtime')),
   'updated_at' DATETIME DEFAULT (datetime('now','localtime'))
);

CREATE TABLE 'fav_products' (
   'placement' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL,
   'product_id' INTEGER
);

CREATE TABLE 'sales' (
   'id' INTEGER PRIMARY KEY  AUTOINCREMENT,
   'user_account_id' INTEGER NOT NULL  check(typeof(user_account_id) = 'integer'),
   'product_id' INTEGER NOT NULL  check(typeof(product_id) = 'integer'),
   'price' FLOAT,
   'created_at' DATETIME DEFAULT (datetime('now','localtime')),
   'updated_at' DATETIME DEFAULT (datetime('now','localtime')),
   FOREIGN KEY(user_account_id) REFERENCES user_accounts(id),
   FOREIGN KEY(product_id) REFERENCES products(id)
);

CREATE TABLE 'user_accounts' (
   'id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL check(typeof(id) = 'integer'),
   'firstname' VARCHAR,
   'lastname' VARCHAR NOT NULL ,
   'status' INTEGER DEFAULT 0,
   'image' BLOB DEFAULT NULL,
   'created_at' DATETIME DEFAULT (datetime('now','localtime')),
   'updated_at' DATETIME DEFAULT (datetime('now','localtime'))
);

CREATE TABLE 'fav_users' (
   'placement' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL,
   'user_account_id' INTEGER
);

CREATE TABLE 'select_bills' ('id' INTEGER PRIMARY KEY  NOT NULL , 'bill_id' INTEGER NOT NULL);
INSERT INTO 'select_bills' (bill_id) VALUES ('1');

CREATE VIEW 'date_last_bills' AS
   SELECT bills.id, MAX(created_at) AS 'created_at', MAX(created_at) AS 'updated_at'
   FROM bills;

CREATE VIEW 'date_second_last_bills' AS
   SELECT bills.id, MAX(bills.created_at) AS 'created_at', MAX(bills.created_at) AS 'updated_at'
   FROM bills, date_last_bills
   WHERE bills.created_at < date_last_bills.created_at;

CREATE VIEW 'dates_selected_bills' AS
	SELECT '1' AS 'id', start_date, end_date FROM 
		(SELECT max(bills.id) as 'id', bills.created_at AS 'start_date' 
			FROM bills, select_bills 
			WHERE bill_id > bills.id AND select_bills.id = '1')
	JOIN
		(SELECT bills.created_at AS 'end_date' 
			FROM bills, select_bills 
			WHERE bill_id = bills.id AND select_bills.id = '1');

CREATE VIEW 'selected_account_purchases' AS
		SELECT sale_id AS 'id', user_account_id, lastname, firstname, product_id, ean, name, price, count(product_id) AS count, round(sum(round(price,2)),2) AS 'sum', created_at,updated_at
			FROM
				(SELECT id AS 'user_id', lastname, firstname
				FROM user_accounts)
			INNER JOIN
				(
				   (SELECT id AS 'pro_id', ean, name FROM products)
				INNER JOIN
				  (SELECT sales.id AS 'sale_id',user_account_id, product_id, price,dates_selected_bills.end_date AS 'created_at', dates_selected_bills.end_date AS 'updated_at'
				   FROM sales,dates_selected_bills
				   WHERE sales.created_at > dates_selected_bills.start_date
				   AND sales.created_at <= dates_selected_bills.end_date)
				ON pro_id = product_id
				)
			ON user_account_id = user_id
			GROUP BY user_account_id, product_id, price
			ORDER BY lastname, firstname, product_id;

CREATE VIEW 'selected_account_balances' AS
   SELECT user_account_id AS 'id', user_account_id, firstname, lastname, round(sum(round(price,2)),2) AS 'sum', created_at, updated_at
   FROM
      (
         (SELECT id AS 'user_id',firstname,lastname
         FROM user_accounts)
      INNER JOIN
         (SELECT user_account_id, product_id, price,dates_selected_bills.end_date AS 'created_at',dates_selected_bills.end_date AS 'updated_at'
         FROM sales, dates_selected_bills
         WHERE sales.created_at > dates_selected_bills.start_date
         AND sales.created_at <= dates_selected_bills.end_date)
      ON user_id =user_account_id)
      GROUP BY user_account_id
      ORDER BY lastname,firstname;

CREATE VIEW 'selected_consumptions' AS
SELECT product_id AS 'id', product_id, ean , name,price, count(user_account_id) AS 'count', round(sum(round(price,2)),2) AS 'sum', created_at, updated_at
   FROM
      (
         (SELECT id AS 'prod_id', ean,name
         FROM products)
      INNER JOIN
         (SELECT user_account_id, product_id, price,dates_selected_bills.end_date AS 'created_at',dates_selected_bills.end_date AS 'updated_at'
         FROM sales, dates_selected_bills
         WHERE sales.created_at > dates_selected_bills.start_date
         AND sales.created_at <= dates_selected_bills.end_date)
      ON prod_id = product_id)
      GROUP BY product_id, price
      ORDER BY name;

CREATE VIEW 'previous_account_purchases' AS
SELECT sale_id AS 'id', user_account_id, lastname, firstname, product_id, ean, name, price, count(product_id) AS count, round(sum(round(price,2)),2) AS 'sum', created_at,updated_at
   FROM
      (SELECT id AS 'user_id', lastname, firstname
      FROM user_accounts)
   INNER JOIN
      (
         (SELECT id AS 'pro_id', ean, name FROM products)
      INNER JOIN
        (SELECT sales.id AS 'sale_id',user_account_id, product_id, price,date_last_bills.created_at, date_last_bills.updated_at
         FROM sales,date_last_bills, date_second_last_bills
         WHERE sales.created_at > date_second_last_bills.created_at
         AND sales.created_at <= date_last_bills.created_at)
      ON pro_id = product_id
      )
   ON user_account_id = user_id
   GROUP BY user_account_id,product_id, price
   ORDER BY lastname, firstname, product_id;

CREATE VIEW 'previous_account_balances' AS
   SELECT user_account_id AS 'id', user_account_id, firstname, lastname, round(sum(round(price,2)),2) AS 'sum', created_at, updated_at
   FROM
      (
         (SELECT id AS 'user_id',firstname,lastname
         FROM user_accounts)
      INNER JOIN
         (SELECT user_account_id, product_id, price,date_last_bills.created_at,date_last_bills.updated_at
         FROM sales, date_last_bills, date_second_last_bills
         WHERE sales.created_at > date_second_last_bills.created_at
         AND sales.created_at <= date_last_bills.created_at)
      ON user_id =user_account_id)
      GROUP BY user_account_id
      ORDER BY lastname,firstname;

CREATE VIEW 'previous_consumptions' AS
SELECT product_id AS 'id', product_id, ean , name,price, count(product_id) AS 'count', round(sum(round(price,2)),2) AS 'sum', created_at, updated_at
   FROM
      (
         (SELECT id AS 'prod_id', ean,name
         FROM products)
      INNER JOIN
         (SELECT user_account_id, product_id, price, date_last_bills.created_at,date_last_bills.updated_at
         FROM sales, date_last_bills,date_second_last_bills
         WHERE sales.created_at > date_second_last_bills.created_at
         AND sales.created_at <= date_last_bills.created_at)
      ON prod_id = product_id)
      GROUP BY product_id, price
      ORDER BY name;

CREATE VIEW 'previous_fav_products' AS
   SELECT product_id, ean, name, num
   FROM ( SELECT id AS 'prod_id', ean, name FROM products) 
      INNER JOIN
         (SELECT product_id, count(product_id) AS num
         FROM
            (SELECT product_id
            FROM sales,date_last_bills, date_second_last_bills
            WHERE  sales.created_at > date_second_last_bills.created_at 
            AND sales.created_at <= date_last_bills.created_at
               AND sales.price > 0)
         GROUP BY product_id
         ORDER BY num desc)
      ON prod_id = product_id;

CREATE VIEW 'previous_fav_users' AS
   SELECT user_account_id,firstname,lastname, sum
   FROM (SELECT id AS 'user_id', firstname, lastname FROM user_accounts)
      INNER JOIN
         (SELECT user_account_id, round(sum(round(price,2)),2) AS sum
         FROM
            (SELECT user_account_id, product_id, price
            FROM sales,date_last_bills, date_second_last_bills
            WHERE  sales.created_at > date_second_last_bills.created_at 
            AND sales.created_at <= date_last_bills.created_at
            AND sales.price > 0)
         GROUP BY user_account_id
         ORDER BY sum desc)
      ON user_id=user_account_id;

CREATE VIEW 'current_account_purchases' AS
SELECT sale_id AS 'id', user_account_id, lastname, firstname, product_id, ean, name, price, count(product_id) AS count, round(sum(round(price,2)),2) AS 'sum', created_at, updated_at
   FROM
      (SELECT id AS 'user_id', lastname, firstname
      FROM user_accounts)
   INNER JOIN
      (
         (SELECT id AS 'prod_id', ean,name FROM products)
      INNER JOIN
        (SELECT sales.id AS 'sale_id',user_account_id, product_id, price,date_last_bills.created_at, date_last_bills.updated_at
         FROM sales,date_last_bills
         WHERE sales.created_at >= date_last_bills.created_at)
      ON prod_id = product_id
      )
   ON user_account_id = user_id
   GROUP BY user_account_id, product_id, price
   ORDER BY lastname,firstname,name;

CREATE VIEW 'current_account_balances' AS
   SELECT user_account_id AS 'id', user_account_id, firstname, lastname, round(sum(round(price,2)),2) AS 'sum', created_at, updated_at
   FROM
      (
         (SELECT id AS 'user_id',lastname,firstname
         FROM user_accounts)
      INNER JOIN
         (SELECT user_account_id, product_id, price, date_last_bills.created_at, date_last_bills.updated_at
         FROM sales,date_last_bills
         WHERE sales.created_at > date_last_bills.created_at)
      ON user_id = user_account_id)
      GROUP BY user_account_id
      ORDER BY lastname,firstname;

CREATE VIEW 'current_consumptions' AS
SELECT product_id AS 'id',product_id, ean, name,price, count(product_id) AS 'count', round(sum(round(price,2)),2) AS 'sum',created_at,updated_at
   FROM
      (
         (SELECT id AS 'prod_id', ean,name
         FROM products )
      INNER JOIN
         (SELECT user_account_id, product_id, price,date_last_bills.created_at,date_last_bills.updated_at
         FROM sales, date_last_bills
         WHERE  sales.created_at > date_last_bills.created_at)
      ON prod_id = product_id)
      GROUP BY product_id,price
      ORDER BY name;

CREATE VIEW 'current_fav_products' AS
   SELECT product_id, ean, name, num
   FROM ( SELECT id AS 'prod_id', ean, name FROM products) 
      INNER JOIN
         (SELECT product_id, count(product_id) AS num
         FROM
            (SELECT product_id
            FROM sales,date_last_bills
            WHERE sales.created_at > date_last_bills.created_at
            AND sales.price > 0)
         GROUP BY product_id
         ORDER BY num desc)
      ON prod_id = product_id;

CREATE VIEW 'current_fav_users' AS
   SELECT user_account_id,firstname,lastname,status,sum
   FROM ( SELECT id AS 'user_id', firstname, lastname,status FROM user_accounts) 
      INNER JOIN
         (SELECT user_account_id, round(sum(round(price,2)),2) AS sum
         FROM
            (SELECT user_account_id, product_id, price
            FROM sales,date_last_bills
            WHERE sales.created_at>date_last_bills.created_at
            AND sales.price > 0)
         GROUP BY user_account_id
         ORDER BY sum desc)
      ON user_id=user_account_id;



CREATE TRIGGER 'make_bill'
   BEFORE INSERT ON 'bills'
   BEGIN
      SELECT CASE
         WHEN NEW.created_at < (SELECT MAX(created_at) FROM bills)
         THEN RAISE(ABORT, 'There is a newer bill date in the table.Try a newer date!')
         WHEN NEW.created_at > (SELECT datetime('now','localtime'))
          THEN RAISE(ABORT, 'Date is in the future, Try an older date!')
      END;
   END;

CREATE TRIGGER 'reload_fav'
   AFTER INSERT ON 'bills'
   BEGIN
      DELETE FROM fav_products;
      INSERT INTO fav_products (product_id) SELECT product_id FROM previous_fav_products;
      DELETE FROM fav_users;
      INSERT INTO fav_users (user_account_id) SELECT user_account_id FROM previous_fav_users;
   END;

CREATE TRIGGER 'rewind_reload_fav'
   AFTER DELETE ON 'bills'
   BEGIN
      DELETE FROM fav_products;
      INSERT INTO fav_products (product_id) SELECT product_id FROM previous_fav_products;
      DELETE FROM fav_users;
      INSERT INTO fav_users (user_account_id) SELECT user_account_id FROM previous_fav_users;
   END;

CREATE TRIGGER 'rewind_seq_bills' AFTER DELETE ON 'bills'
   BEGIN
      UPDATE sqlite_sequence SET seq=(SELECT MAX(id) FROM bills) WHERE name = 'bills';
   END;

CREATE TRIGGER 'rewind_seq_products'
   AFTER DELETE ON 'fav_products'
   BEGIN
      UPDATE sqlite_sequence SET seq=0 WHERE name = 'fav_products';
   END;

CREATE TRIGGER 'rewind_seq_user' AFTER DELETE ON 'fav_users'
   BEGIN
      UPDATE sqlite_sequence SET seq=0 WHERE name = 'fav_users';
   END;

CREATE TRIGGER 'check_delete_product'
   BEFORE DELETE ON 'products'
   FOR EACH ROW
   BEGIN
      SELECT CASE
         WHEN OLD.id IN (SELECT product_id FROM current_consumptions)
         THEN RAISE(ABORT, 'Some unpaid products left in current_consumption. Please make a new bill first.')
         WHEN OLD.id IN (SELECT product_id FROM previous_consumptions)
         THEN RAISE(ABORT, 'Some products left in previous_consumption. Even if you exported the bill already, deletion will be not possile until next bill.')
      END;
      DELETE FROM sales WHERE OLD.id = sales.product;
      DELETE FROM fav_products;
      INSERT INTO fav_products (product_id) SELECT product_id FROM previous_fav_products;
   END;

CREATE TRIGGER 'check_delete_user'
   BEFORE DELETE ON 'user_accounts'
   FOR EACH ROW
   BEGIN
      SELECT CASE
         WHEN OLD.id IN (SELECT user_account_id FROM current_account_balances)
         THEN RAISE(ABORT, 'Some unpaid products left in current_account_balance. Please make a new bill first.')
         WHEN OLD.id IN (SELECT user_account_id FROM previous_account_balances)
        THEN RAISE(ABORT, 'Some products left in previous_account_balance. Even if you exported the bill already, deletion will be not possile until next bill.')
      END;
      DELETE FROM sales WHERE OLD.id = sales.userid;
      DELETE FROM fav_users;
      INSERT INTO fav_users (user_account_id) SELECT user_account_id FROM previous_fav_users;
   END;

CREATE TRIGGER 'check_delete_sales'
   BEFORE DELETE ON 'sales'
   FOR EACH ROW
   BEGIN
      SELECT CASE
         WHEN OLD.created_at > (SELECT created_at FROM date_last_bills)
         THEN RAISE(ABORT, 'Cant delete sale, please make a new bill first. If you want to cancel the sale, set the price of sale to 0.00')
         WHEN OLD.created_at > (SELECT created_at FROM date_second_last_bills)
         THEN RAISE(ABORT, 'Cant delete sale. Deletion will be not possile until next bill. If you exported the bill already and want to cancel the sale, set price of sale to 0.00 and export again.')
      END;
   END;
