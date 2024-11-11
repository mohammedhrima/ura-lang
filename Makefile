all:
	docker-compose up -d

down:
	docker-compose down --rmi all

clean: down
	docker system prune -a -f
	docker volume prune -f
	docker image prune -f
	docker network prune -f
	docker container prune -f
	dangling_volumes=$$(docker volume ls -q --filter dangling=true); \
    if [ $$? -eq 0 ]; then \
        for volume in $$dangling_volumes; do \
            docker volume rm $$volume; \
        done; \
    fi

re: clean all