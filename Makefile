COMPOSE = docker-compose -f docker/docker-compose.yml

all:
	$(COMPOSE) up -d

down:
	$(COMPOSE) down

clean: down
	docker volume rm $$(docker volume ls -q --filter name=$$(basename $$PWD)_*)

fclean: down
	docker system prune -af
	docker volume prune -f
	docker network prune -f

logs:
	$(COMPOSE) logs -f

re: clean all
