# Variables
COMPOSE = docker-compose -f docker/docker-compose.yml
PROJECT_NAME = $(shell basename $(PWD))

# Targets
all: up

up:
	# echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
	@echo "Starting Docker containers..."
	$(COMPOSE) up --build -d

down:
	@echo "Stopping Docker containers..."
	$(COMPOSE) down

clean: down
	@echo "Removing Docker volumes..."
	docker volume rm -f $$(docker volume ls -q --filter name=$(PROJECT_NAME)_*)

fclean: down
	@echo "Cleaning Docker system..."
	docker system prune -af
	docker volume prune -f
	docker network prune -f

logs:
	@echo "Showing logs..."
	$(COMPOSE) logs -f

re: clean all

.PHONY: all build up down clean fclean logs re