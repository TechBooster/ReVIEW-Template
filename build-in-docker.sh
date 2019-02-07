#!/bin/bash

# コマンド手打ちで作業したい時は以下の通り /book に pwd がマウントされます
# docker run -i -t -v $(pwd):/book vvakame/review:3.0 /bin/bash

docker run -t --rm -v $(pwd):/book vvakame/review:3.0 /bin/bash -ci "cd /book && ./setup.sh && npm run pdf"
