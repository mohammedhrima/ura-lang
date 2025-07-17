# Project Variables
COMPOSE = docker-compose -f docker/docker-compose.yml
CONTAINER = wcc

# Start everything
all: up

# Build and start the Docker container (x86_64)
up:
	@echo "🚀 Building and starting Docker container..."
	$(COMPOSE) build --no-cache
	$(COMPOSE) up -d

# Stop container
down:
	@echo "🛑 Stopping Docker container..."
	$(COMPOSE) down

# Clean volumes
clean: down
	@echo "🧹 Cleaning volumes..."
	docker volume rm -f $$(docker volume ls -q --filter name=wcc)

# Remove everything
fclean: down
	@echo "🔥 Pruning Docker system..."
	docker system prune -af
	docker volume prune -f
	docker network prune -f

# Rebuild project
re: fclean all

# Exec into the container shell
exec:
	docker exec -it $(CONTAINER) bash

# Compile wcc inside the container using setup.sh logic
build:
	docker exec -it $(CONTAINER) bash -c "build"

# Run file.s using system gcc and execute the result
run:
	docker exec -it $(CONTAINER) bash -c "run"

# Run setup.sh 'copy' function: copy generated .w and .s files to test suite
copy:
	docker exec -it $(CONTAINER) bash -c "copy default"

# Run all tests from /tests
test:
	docker exec -it $(CONTAINER) bash -c "test"

# Format code inside container
indent:
	docker exec -it $(CONTAINER) bash -c "indent"

# Re-source .bashrc to load aliases and env
update:
	docker exec -it $(CONTAINER) bash -c "update"

# Show logs
logs:
	$(COMPOSE) logs -f
