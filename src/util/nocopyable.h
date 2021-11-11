
namespace lite_http {
    /**
      禁用拷贝复制
     */
    class nocopyable {
    public:
        nocopyable() = default;
        ~nocopyable() = default;
        nocopyable(const nocopyable&) = delete;
        nocopyable& operator=(const nocopyable&) = delete;
    };
}
