#!/bin/bash

publisher_path="/home/jastorj/Workspace/CS744/project/demo_pub_sub_apps/load-publisher"
subscriber_path="/home/jastorj/Workspace/CS744/project/demo_pub_sub_apps/normal-subscriber"

output_dir="output"
mkdir $output_dir 2>/dev/null

topic_name="test_topic_1"
subscription_name="test_topic_1_sub_1"
message_count=150

$publisher_path/bin/main $message_count $topic_name > "$output_dir/publisher_output_$message_count.tsv" &
$subscriber_path/bin/main $topic_name $subscription_name > "$output_dir/subscriber_output_$message_count.tsv" &
wait
