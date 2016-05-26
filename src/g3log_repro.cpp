// g3log
#include <g3sinks/LogRotate.h>
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include <g3log/std2_make_unique.hpp>

#include <boost/filesystem.hpp>

int main(int argc, char* argv[]) {
	const char* path = "repro";
	boost::filesystem::path dir(path);
    boost::filesystem::create_directory(dir);

	std::unique_ptr<g3::LogWorker> worker(g3::LogWorker::createLogWorker());
	auto logger = worker->addSink(std2::make_unique<LogRotate>(argv[0], path),
                                  &LogRotate::save);
    g3::initializeLogging(worker.get());

    size_t count = 0;
    while(true) {
    	LOG(INFO) << std::to_string(count++) << std::endl;
    }
}