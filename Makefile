IMAGE     := ura-dev
CONTAINER := ura-lang
MOUNT     := /ura-lang
PLATFORM  := linux/amd64
ANVIL_BIN := config/anvil/build/linux/anvil

.PHONY: image re shell stop clean anvil

image:
	@docker image inspect $(IMAGE) >/dev/null 2>&1 || \
	    docker build --platform $(PLATFORM) -t $(IMAGE) .

re:
	-docker rm -f $(CONTAINER)
	docker build --platform $(PLATFORM) -t $(IMAGE) .

$(ANVIL_BIN): | image
	docker run --rm --platform $(PLATFORM) \
	    -v "$(CURDIR):$(MOUNT)" \
	    -w $(MOUNT)/config/anvil \
	    $(IMAGE) make

anvil: image
	docker run --rm --platform $(PLATFORM) \
	    -v "$(CURDIR):$(MOUNT)" \
	    -w $(MOUNT)/config/anvil \
	    $(IMAGE) make re

shell: image $(ANVIL_BIN)
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
