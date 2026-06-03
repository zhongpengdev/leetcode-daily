```bash
sudo apt update
sudo apt install nginx -y

sudo systemctl start nginx

# 开机自启
sudo systemctl enable nginx

# 状态
sudo systemctl status nginx

nginx -v

```

# resume file location 

```bash
sudo mkdir -p /var/www/resume

sudo nano /etc/nginx/sites-available/resume

server {
    listen 80;

    server_name cv.yourdomain.com;

    location / {
        alias /var/www/resume/;
        autoindex off;
    }
}

# start
sudo ln -s \
/etc/nginx/sites-available/resume \
/etc/nginx/sites-enabled/

sudo nginx -t

# reload
sudo systemctl reload nginx
```

