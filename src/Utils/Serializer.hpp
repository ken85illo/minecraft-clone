#pragma once

class Serializer {
public:
    static bool fileExists(const std::string &filename) {
        using namespace std::literals::string_literals;

        std::filesystem::path filepath = "./data/"s + filename + ".dat"s;
        return std::filesystem::exists(filepath);
    }

    template <typename Serializable>
    static void write(const Serializable &s, const std::string &filename) {
        using namespace std::literals::string_literals;

        std::filesystem::path dir = "./data/";
        if (!std::filesystem::exists(dir)) {
            std::filesystem::create_directory(dir);
        }

        std::string filepath = dir.string() + filename + ".dat"s;
        std::ofstream file(filepath, std::ios::binary);

        if (!file.is_open()) {
            std::println("Error: Cannot open/write file.");
            return;
        }

        file.write(reinterpret_cast<const char *>(&s), sizeof(s));
        file.close();
        std::println("{}: Object has been successfully serialized.", filepath);
    }

    template <typename Serializable>
    static Serializable *read(const std::string &filename) {
        using namespace std::literals::string_literals;

        std::string filepath = "./data/"s + filename + ".dat"s;
        std::ifstream file(filepath, std::ios::binary);

        if (!file.is_open()) {
            std::println("Error: Cannot open/read file.");
            return nullptr;
        }

        Serializable *s = new Serializable;
        file.read(reinterpret_cast<char *>(s), sizeof(*s));
        file.close();
        std::println("{}: Object has been successfully deserialized.", filepath);
        return s;
    }
};
