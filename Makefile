all:
	docker-compose up -d

down:
	docker-compose down --rmi all

clean: down
	docker system prune -af
	-docker volume rm $$(docker volume ls -q)

re: clean all
