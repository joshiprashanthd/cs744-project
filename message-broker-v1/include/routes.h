#pragma once

#include "iostream"

const std::string SERVICE_REGISTER_URL = "/services/register";
const std::string SERVICE_TOPIC_PUBLISH_URL = "/services/*/topics/publish";
const std::string SERVICE_SEND_MESSAGE_URL = "/services/*/topics/*/send";
const std::string SERVICE_TOPIC_SUBSCRIBE_URL = "/services/*/subscriptions/subscribe";
const std::string SERVICE_SUBSCRIPTION_PULL_MESSAGES_URL = "/services/*/topics/*/subscriptions/*/messages";
