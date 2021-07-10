FROM conanio/clang11 as builder
USER root
WORKDIR /proj
COPY src ./src
COPY CMakeLists.txt ./CMakeLists.txt
RUN mkdir ./res
RUN cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B ./res
RUN cmake --build ./res --target all

FROM debian as backend
COPY --from=builder /proj/res/app /app
ENTRYPOINT ["/app"]
