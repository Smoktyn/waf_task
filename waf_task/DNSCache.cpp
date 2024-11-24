#include <DNSCache.hpp>
#include <iostream>
#include <cassert>

int main() {

    try {
        auto& cache = DNSCache::instance(3);

        {
            cache.update("example.com", "192.168.1.1");
            assert(cache.resolve("example.com") == "192.168.1.1");

            cache.update("example.com", "192.168.1.2");
            assert(cache.resolve("example.com") == "192.168.1.2");
        }

        {
            assert(cache.resolve("nonexistent.com") == "");
        }

        {
            cache.update("example1.com", "192.168.1.1");
            cache.update("example2.com", "192.168.1.2");
            cache.update("example3.com", "192.168.1.3");

            cache.update("example4.com", "192.168.1.4");

            assert(cache.resolve("example1.com") == "");
            assert(cache.resolve("example2.com") == "192.168.1.2");
            assert(cache.resolve("example3.com") == "192.168.1.3");
            assert(cache.resolve("example4.com") == "192.168.1.4");
        }

        {
            cache.update("example5.com", "192.168.1.5");
            cache.update("example6.com", "192.168.1.6");
            cache.update("example7.com", "192.168.1.7");

            cache.resolve("example5.com");
            cache.update("example8.com", "192.168.1.8");

            assert(cache.resolve("example5.com") == "192.168.1.5");
            assert(cache.resolve("example6.com") == "");
            assert(cache.resolve("example7.com") == "192.168.1.7");
            assert(cache.resolve("example8.com") == "192.168.1.8");
        }

		std::cout << "All tests passed" << std::endl;
	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}