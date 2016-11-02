COMMIT="完善了makefile和目录结构(memfi+ptracefi)..."

github:
	git add -A
	git commit -m $(COMMIT)
	git push origin master
