FROM vvakame/review:5.0

ENV APP_HOME /book
RUN mkdir -p $APP_HOME
WORKDIR $APP_HOME

ADD Gemfile Gemfile
ADD package.json package.json
ADD package-lock.json package-lock.json

RUN gem install bundler:1.17.2
RUN npm install --unsafe-perm
