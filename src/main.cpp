#include "httplib.h"
#include <dirent.h> 
#include <string>
#include "cJSON.h"
#include "common.h"
#include <fstream>
#include "PropertiesParser.h"
#include "json.hpp"
#include <dlfcn.h>
#include "dl.h"

#include "fmt/core.h"

using nlohmann::json;

enum HTTP_STATUS_CODE {
	HTTP_E_BAD_REQUEST	= 400,
	HTTP_E_INTERNAL 	= 500,
};

void invoke_dl2() {
	void* handle = dlopen("libdl2.so", RTLD_LAZY);  // 打开共享库文件

    if (handle != nullptr)
    {
        typedef DL* (*GetInstanceFunc)();  // 定义函数指针类型
		typedef void (*Method1Func)(DL*);
		typedef void (DL::*Method1Func2)();
        GetInstanceFunc getInstance = (GetInstanceFunc)dlsym(handle, "_ZN2DL11getInstanceEv");  // 获取 getInstance 函数指针

        if (getInstance != nullptr)
        {
            DL* instance = getInstance();  // 调用 getInstance 方法获取实例
            Method1Func method1 = nullptr;
            *(void**)(&method1) = dlsym(handle, "method1");  // 获取 method1 成员函数指针
			if (method1 != nullptr)
            {
                method1(instance);  // 调用 method1 方法
            }
            else
            {
                fmt::print("Failed to get function pointer for method1.\n");
            }
			Method1Func2 method12 = nullptr;
            *(void**)(&method12) = dlsym(handle, "_ZN2DL7method1Ev");  // 获取 method1 成员函数指针
			if (method12 != nullptr)
            {
                (instance->*method12)();  // 调用 method1 方法
            }
            else
            {
                fmt::print("Failed to get function pointer for method12.\n");
            }

            dlclose(handle);  // 关闭共享库
        }
    } else {
		fmt::print("Failed to open shared library: ", dlerror(), "\n");
	}
}

int main(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		invoke_method("libm.so.6", argv[i], 1.0);
	}
	invoke_dl2();

	httplib::Server svr;

	// use nlohmann/json
	json datas = json::parse(R"(
		{
			"pi": 3.141,
			"happy": true,
			"arr": [
				{
					"key1": "val1",
					"key2": "val2"
				},
				{
					"key3": "val3",
					"key4": "val4"
				}
			],
			"name": "Niels",
			"nothing": null,
			"answer": {
				"everything": 42
			},
			"object": {
				"currency": "USD",
				"value": 42.99
			}
		}
	)");
	svr.Get("/datas/expel", [&datas](const httplib::Request &req, httplib::Response &res) {
		res.set_content(datas.dump(4), "application/json");
	});
	svr.Post("/datas/swallow", [&datas](const httplib::Request &req, httplib::Response &res) {
		try {
			datas = json::parse(req.body);
			fmt::print(datas.dump(4), "\n");
		} catch (json::parse_error& e){
			res.status = HTTP_E_BAD_REQUEST;
		}
	});

	// scripts related http api
	// 获取脚本
	svr.Get("/scripts", [](const httplib::Request &req, httplib::Response &res) {
			DIR *dr;
			struct dirent *en;
			dr = opendir("../scripts"); //open all directory
			if (dr) {
			std::string content;
			while ((en = readdir(dr)) != NULL) {
			if (en->d_type == DT_REG) {
			if (!content.empty()) content += ", ";
			content.append("\"" + std::string(en->d_name) + "\"");
			fmt::print(en->d_name, "\n"); //print all normal file name
			}
			}
			closedir(dr); //close all directory
			res.set_content("{\"result\": 0, [" + content + "]", "application/json");
			} else {
			fmt::print("open directory error\n");
			res.status = HTTP_E_INTERNAL;
			}

			});
	// 新建脚本
	svr.Post("/scripts", [](const httplib::Request &req, httplib::Response &res){
			cJSON *item = NULL, *subItem = NULL;
			item = cJSON_Parse(req.body.c_str());
			if(!item) {
			res.status = HTTP_E_BAD_REQUEST;
			return;
			}
			subItem = cJSON_GetObjectItem(item, "name");
			if(!subItem) { cJSON_free(item); }
			std::string script_name = cJSON_GetStringValue(subItem);

			subItem = cJSON_GetObjectItem(item, "content");
			if(!subItem) { cJSON_free(item); }
			std::string script_content = cJSON_GetStringValue(subItem);


			std::ofstream f_script;
			f_script.open(str_format("../scripts/%s", script_name.c_str()), std::ios::trunc);
			if(!f_script.is_open()) {
			perror("the cause of failing to open scripts directory");
			return;
			} else {
				f_script << script_content;
			}
			f_script.close();

	});
	// 删除脚本
	svr.Delete("/scripts/:name", [](const httplib::Request &req, httplib::Response &res){
			auto name = req.path_params.at("name").c_str();

			if (std::remove(str_format("../scripts/%s", name).c_str()) == 0) {
			res.set_content("{\"result\": 0}",	"application/json");
			} else {
			res.status = HTTP_E_BAD_REQUEST;
			}

			});
	// 更新脚本
	svr.Put("/scripts/:name", [](const httplib::Request &req, httplib::Response &res){
			auto name = req.path_params.at("name").c_str();
			cJSON *item = NULL, *subItem = NULL;
			item = cJSON_Parse(req.body.c_str());
			if(!item) {
			res.status = HTTP_E_BAD_REQUEST;
			return;
			}

			subItem = cJSON_GetObjectItem(item, "content");
			if(!subItem) { cJSON_free(item); }
			std::string script_content = cJSON_GetStringValue(subItem);

			std::ofstream f_script;
			f_script.open(str_format("../scripts/%s", name), std::ios::trunc);
			if(!f_script.is_open()) {
			perror("the cause of failing to open scripts directory");
			return;
			} else {
			f_script << script_content;
			}
			f_script.close();

	});

	auto ret = svr.set_mount_point("/scripts", "../scripts");
	if (!ret) {
		fmt::print("the scripts directory doesn't exist...\n");
	}
	cppproperties::Properties props = cppproperties::PropertiesParser::Read("../config/application.properties");
	svr.listen(props.GetProperty("ip").c_str(), stoi(props.GetProperty("port")));

	return 0;
}
