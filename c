make clean
/usr/bin/phpize
./configure --with-php-config=/usr/bin/php-config
make
make install
#php -m
php tests/nii_base_component.phpt
