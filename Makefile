IMAGE := ura-dev

.PHONY: image shell anvil

image:
	docker build --platform linux/amd64 -t $(IMAGE) .

shell: image
	docker run --platform linux/amd64 --rm -it -v "$(CURDIR):/workspace" $(IMAGE)

# Rebuild anvil inside the container from the mounted source and reinstall it.
# Handy when iterating on anvil C++ without re-baking the whole image.
anvil:
	docker run --platform linux/amd64 --rm -v "$(CURDIR):/workspace" $(IMAGE) \
	    sh -c 'make -C /workspace/config/anvil && install -m 755 /workspace/config/anvil/anvil /usr/local/bin/anvil'
