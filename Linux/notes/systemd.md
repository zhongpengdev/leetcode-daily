用来管理应用程序的

查看服务状态
sudo systemctl status nginx

启动服务
sudo systemctl start nginx

停止服务
sudo systemctl stop nginx

重启服务
sudo systemctl restart nginx

热重启，不中断
sudo systemctl reload nginx

---

开机自启
sudo systemctl enable nginx

取消开机自启
sudo systemctl disable nginx

启动并设置开机自启
sudo systemctl start --now nginx
