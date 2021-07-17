FROM conanio/clang11 as builder
USER root
WORKDIR /proj
COPY src ./src
COPY CMakeLists.txt ./CMakeLists.txt
RUN cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B ./bin
RUN cmake --build ./bin --target all

FROM debian as backend
COPY --from=builder /proj/bin/app /app
ENTRYPOINT ["/app"]
