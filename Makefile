COMMIT="完善了脚本, 修复了ptrace的bug..."

github:
	git add -A
	git commit -m $(COMMIT)
	git push origin master
