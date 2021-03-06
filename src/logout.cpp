/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   logout.cpp
 * Author: centos7
 * 
 * Created on November 15, 2016, 9:32 PM
 */

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/URI.h>

#include "logout.hpp"

namespace webcppd {
    namespace blog {

        void logout::do_get(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            if (!this->session_has(request, response, "user.id")) {
                response.redirect("/blog/login");
                return;
            }
            this->session_remove(request, response, "user.id");
            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html;charset=utf-8");

            std::string cacheKey(this->create_cache_key(request, response));


            if (root_view::root_cache().has(cacheKey)) {
                response.send() << *root_view::root_cache().get(cacheKey);
                return;
            }

            Poco::SharedPtr<Kainjow::Mustache::Data> data = this->tpl_ready("/blog/config.json", "user_logout.GET");


            root_view::root_cache().add(cacheKey, this->render_tpl(data->get("maintpl")->stringValue(), *data));
            response.send() << *root_view::root_cache().get(cacheKey);
        }


    }
}

