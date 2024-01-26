all:
	docker-compose up --build -d

down:
	docker-compose down

clean: down
	docker system prune -a -f
	docker volume prune -f
	docker image prune -f
	docker network prune -f

re: clean all