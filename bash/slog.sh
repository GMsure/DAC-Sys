id=$1
min=$2m
echo $min
sudo docker logs --since $min $id
