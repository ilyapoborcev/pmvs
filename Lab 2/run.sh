source config.ini
while :
do
wget -q -O - http://6.pogoda.by/ |
iconv -f WINDOWS-1251 -t UTF-8 |
tr '\n' ' ' |
grep -Poa '<div id=\"content\">(.*?)C' |
grep -Poa '[0-9]+\.[0-9]'
sleep $sleeptime
done
