#pragma once

#include <fcntl.h>
#include <fstream>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

namespace pumpkin {

template <typename T>
class Configuration {

public:
	
	Configuration(const std::string & conf_file) {
		parse(conf_file);	       
	}

	Configuration(const T & conf) {
		m_config = conf;
	}

	const T & config() const { return m_config; }

private:

	bool parse(const std::string & conf_file) {

		int fileDescriptor = open(conf_file.c_str(), O_RDONLY);
		google::protobuf::io::FileInputStream fileInput(fileDescriptor);
		fileInput.SetCloseOnDelete( true );

		if (!google::protobuf::TextFormat::Parse(&fileInput, &m_config))
		{
			//log
			std::cout << "Failed to parse file!" << std::endl;
			return false;
		}

		return true;
	}

protected:

	T m_config;
};

}
