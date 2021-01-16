#!/bin/bash
[ ! -z $REVIEW_CONFIG_FILE ] || REVIEW_CONFIG_FILE=config.yml

# コマンド手打ちで作業したい時は以下の通り /book に pwd がマウントされます
# docker run -i -t -v $(pwd):/book vvakame/review:5.0 /bin/bash

#docker run -t --rm -v $(pwd):/book vvakame/review:5.0 /bin/bash -ci "cd /book && ./setup.sh && REVIEW_CONFIG_FILE=$REVIEW_CONFIG_FILE npm run pdf"
docker-compose run --rm review /bin/bash -ci "cd /book && REVIEW_CONFIG_FILE=$REVIEW_CONFIG_FILE npm run pdf"
