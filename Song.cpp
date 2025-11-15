#include "Song.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

namespace {
// 去除首尾空白
std::string trim_copy(const std::string &s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

// 转小写
std::string to_lower_copy(const std::string &s) {
    std::string res = s;
    std::transform(res.begin(), res.end(), res.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return res;
}

// 将标签向量拼接成逗号分隔字符串
std::string join_tags(const std::vector<std::string> &tags) {
    std::ostringstream oss;
    for (size_t i = 0; i < tags.size(); ++i) {
        if (i != 0) oss << ", ";
        oss << tags[i];
    }
    return oss.str();
}
} // namespace

int Song::next_id_ = 1;

Song::Song(const std::string &title, const std::string &artist, int duration_sec, int rating)
    : rating_(rating) {
    std::string t = trim_copy(title);
    std::string a = trim_copy(artist);

    if (t.empty()) { std::cout << "[错误] 标题不能为空\n"; return; }
    if (a.empty()) { std::cout << "[错误] 艺人不能为空\n"; return; }
    if (duration_sec <= 0) { std::cout << "[错误] 时长必须为正整数（秒）\n"; return; }
    if (rating < 1 || rating > 5) { std::cout << "[错误] 评分必须在 1...5 之间\n"; return; }

    id_ = next_id_++;
    title_ = t;
    artist_ = a;
    duration_sec_ = duration_sec;
    rating_ = rating;
    valid_ = true;
}

bool Song::set_title(const std::string &t) {
    std::string temp = trim_copy(t);
    if (temp.empty()) { std::cout << "[提示] 标题不能为空，已忽略本次修改\n"; return false; }
    title_ = temp;
    return true;
}

bool Song::set_artist(const std::string &a) {
    std::string temp = trim_copy(a);
    if (temp.empty()) { std::cout << "[提示] 艺人不能为空，已忽略本次修改\n"; return false; }
    artist_ = temp;
    return true;
}

bool Song::set_duration(int sec) {
    if (sec <= 0) { std::cout << "[提示] 时长需为正整数，已忽略本次修改\n"; return false; }
    duration_sec_ = sec;
    return true;
}

bool Song::set_rating(int r) {
    if (r < 1 || r > 5) { std::cout << "[提示] 评分需在 1..5，已忽略本次修改\n"; return false; }
    rating_ = r;
    return true;
}

bool Song::add_tag(const std::string &tag) {
    std::string temp = trim_copy(tag);
    if (temp.empty()) { std::cout << "[提示] 空标签已忽略\n"; return false; }
    std::string tempLower = to_lower_copy(temp);
    for (const auto &tg : tags_) {
        if (to_lower_copy(tg) == tempLower) {
            std::cout << "[提示] 标签已存在（忽略大小写）\n";
            return false;
        }
    }
    tags_.push_back(temp);
    return true;
}

bool Song::remove_tag(const std::string &tag) {
    std::string tempLower = to_lower_copy(trim_copy(tag));
    for (size_t i = 0; i < tags_.size(); ++i) {
        if (to_lower_copy(tags_[i]) == tempLower) {
            tags_.erase(tags_.begin() + i);
            return true;
        }
    }
    std::cout << "[提示] 未找到该标签\n";
    return false;
}

bool Song::matches_keyword(const std::string &kw) const {
    std::string key = to_lower_copy(trim_copy(kw));
    if (key.empty()) return false;

    if (to_lower_copy(title_).find(key) != std::string::npos) return true;
    if (to_lower_copy(artist_).find(key) != std::string::npos) return true;

    for (const auto &tg : tags_) {
        if (to_lower_copy(tg).find(key) != std::string::npos) return true;
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const Song &s) {
    os << "[#" << s.id_ << "] " << s.artist_ << " - " << s.title_
       << " (" << s.duration_sec_ << "s) ";
    os << std::string(s.rating_, '*');
    if (!s.tags_.empty()) os << " [tags: " << join_tags(s.tags_) << "]";
    return os;
}

bool operator<(const Song &a, const Song &b) {
    if (a.rating_ > b.rating_) return true;
    if (a.rating_ < b.rating_) return false;
    if (a.title_ < b.title_) return true;
    if (a.title_ > b.title_) return false;
    return a.id_ < b.id_;
}
