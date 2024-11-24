#!/bin/bash

publisher_path="/home/jastorj/Workspace/CS744/project/demo_pub_sub_apps/load-publisher"
subscriber_path="/home/jastorj/Workspace/CS744/project/demo_pub_sub_apps/event-subscriber"

output_dir="output"
mkdir $output_dir 2>/dev/null

topic_name="test_topic_1"
subscription_name="test_topic_1_sub_1"
if [ -z "$1" ]; then
    echo "Usage: $0 <message_count>"
    exit 1
fi

message_count=$(($1 + 0))

$publisher_path/bin/main $message_count $topic_name > "$output_dir/publisher_output_$message_count.tsv" &
$subscriber_path/bin/main > "$output_dir/subscriber_output_$message_count.tsv"
