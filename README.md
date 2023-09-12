a simple http server written with httplib. And maybe a better meson example?
# API
```shell
# 获取所有脚本
curl -i http://localhost:8080/scripts
# 查看脚本test1
curl -i http://localhost:8080/scripts/test1
# 新建脚本test3
curl -i -X POST -H "Content-Type: application/json" -d '{"name":"test3", "content":"content"}' http://localhost:8080/scripts
# 更新脚本test3
curl -i -X PUT -H "Content-Type: application/json" -d '{"content":"another content"}' http://localhost:8080/scripts/test3
# 删除脚本test3
curl -i -X DELETE http://localhost:8080/scripts/test3


# 吐出上一次吞下的json数据
curl -i http://localhost:8080/datas/expel
# 吞下任意json数据
curl -i -X POST -H "Content-Type: application/json" -d '{"name":"name", "content":"content"}' http://localhost:8080/datas/swallow
```

# TODO
- Add Test Case.
