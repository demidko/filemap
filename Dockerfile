FROM conanio/clang11 as builder
WORKDIR /proj
COPY src ./src
COPY CMakeLists.txt ./CMakeLists.txt
RUN mkdir ./release
RUN cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B ./release
RUN cmake --build ./release --target all

FROM debian as backend
COPY --from=builder /proj/release/app /app
ENTRYPOINT ["/app"]
