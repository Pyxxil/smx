#include <iostream>
#include <thread>

#include "http/request.hpp"
#include "http/response.hpp"
#include "http/server.hpp"

#include "smtp/client.hpp"
#include "smtp/common.hpp"
#include "smtp/server.hpp"

auto main(int argc, char **argv) -> int {
  auto smtpServer = smtp::Server();

  auto httpServer =
      http::Server()
          // I'm not actually sure what port this should run on ...
          .bind(3000)
          // I don't think we actually need path's in this format, but it
          // was fun setting this up
          .attach("/", [](const http::Request &request,
                          http::Response &response) {
            auto &&from = request.header("From");
            if (!from) {
              response.status(http::Status::BadRequest)
                  .send("A 'From' header is required");
              return;
            }

            auto &&to = request.header("To");
            if (!to) {
              response.status(http::Status::BadRequest)
                  .send("A 'To' header is required");
              return;
            }

            auto client = smtp::Client().sender(from->at(0));

            for (auto &&recipient : *to) {
              client.add_recipient(recipient);
            }

            auto &&subject = request.header("Subject");
            if (subject) {
              client.subject(subject->at(0));
            }

            auto &&cc = request.header("Cc");
            if (cc) {
              for (auto &&to : *cc) {
                client.cc(to);
              }
            }

            client.on(smtp::Event::ConnectionClosed, [&response](
                                                         smtp::Response &res) {
              if (res.ok()) {
                response.status(http::Status::OK).send(res.response());
              } else {
                response.status(http::Status::BadRequest).send(res.response());
              }
            });

            client.send(request.body());
          });

  auto httpThread = std::thread(&http::Server::run, &httpServer);
  auto smtpThread = std::thread(&smtp::Server::run, &smtpServer);

  httpThread.join();

  return 0;
}