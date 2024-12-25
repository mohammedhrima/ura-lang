all:
	docker-compose up -d

down:
	docker-compose down

clean: down
	docker volume rm $$(docker volume ls -q --filter name=$$(basename $$PWD)_*)

fclean: down
	docker system prune -af
	docker volume prune -f
	docker network prune -f

logs:
	docker-compose logs -f

re: clean all
