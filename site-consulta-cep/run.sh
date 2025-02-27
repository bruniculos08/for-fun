sudo systemctl start httpd.service
sudo mkdir /var/www/html/cep_search_website >>log.txt 2>&1
sudo cp *.[^sh]* /var/www/html/cep_search_website