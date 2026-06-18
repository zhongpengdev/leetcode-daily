import psycopg2

conn = psycopg2.connect(
    host="localhost",
    port=5432,
    dbname="app",
    user="postgres",
    password="12345"
)

cur = conn.cursor()

print(cur)