.DEFAULT_GOAL := all
TOKEN ?= submit
SHELL := /bin/bash

all:
	@if [[ ! -e build/Makefile ]]; then \
		mkdir -p build; \
		cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug; fi
	@make -C build
	
map=maps/level$(LVL)/level$(LVL)-$(NUM).txt

run:
	@../QtOvercooked/QtOvercooked.exe -l $(map) -p build/main.exe

fast:
	@../QtOvercooked/runner.exe -l $(map) -p build/main.exe

play:
	@../QtOvercooked/QtOvercooked.exe -l $(map)

clean:
	@if [[ -d build ]]; then \
		rm -r build; fi

submit:
	$(eval TEMP := $(shell mktemp -d))
	$(eval BASE := $(shell basename $(CURDIR)))
	$(eval FILE := ${TEMP}/${TOKEN}.zip)
	@cd .. && zip -qr ${FILE} ${BASE}/.git
	@echo "Created submission archive ${FILE}"
	@curl -m 5 -w "\n" -X POST -F "TOKEN=${TOKEN}" -F "FILE=@${FILE}" \
		https://exam.problemsolving.top:8085/api/v2/submission/lab
	@rm -r ${TEMP}

