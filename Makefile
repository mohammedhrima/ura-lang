all:
	docker-compose up  -d

down:
	docker-compose down --rmi all

clean: down
	docker system prune -a -f
	docker volume prune -f
	docker image prune -f
	docker network prune -f

re: clean all