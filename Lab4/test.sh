sudo rm -f /dev/fibonacci
sudo rmmod fibonacci
make
sudo insmod fibonacci.ko
sudo mknod /dev/fibonacci c 249 0
sudo chmod a+r+w /dev/fibonacci

echo "\n"
echo "10" > /dev/fibonacci
cat /dev/fibonacci

echo "\n"
echo "11" > /dev/fibonacci
cat /dev/fibonacci

echo "\n"
echo "12" > /dev/fibonacci
cat /dev/fibonacci
echo "\n"

sudo rm -f /dev/fibonacci
sudo rmmod fibonacci
make clean
