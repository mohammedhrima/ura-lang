IMAGE     := ura-dev
CONTAINER := ura-lang
MOUNT     := /ura-lang
PLATFORM  := linux/amd64

.PHONY: image rebuild shell stop clean

# Build the image only if it doesn't exist. Use `make rebuild` to force.
image:
	@docker image inspect $(IMAGE) >/dev/null 2>&1 || \
	    docker build --platform $(PLATFORM) -t $(IMAGE) .

rebuild:
	docker build --platform $(PLATFORM) -t $(IMAGE) .

# Create the container once; reuse it on subsequent `make shell` calls.
shell: image
	@if docker container inspect $(CONTAINER) >/dev/null 2>&1; then \
	    docker start -ai $(CONTAINER); \
	else \
	    docker run --platform $(PLATFORM) -it \
	        --name $(CONTAINER) \
	        -v "$(CURDIR):$(MOUNT)" \
	        -w $(MOUNT) \
	        $(IMAGE); \
	fi

stop:
	-docker stop $(CONTAINER)

clean:
	-docker rm -f $(CONTAINER)
	-docker rmi $(IMAGE)
