#include "htmlwatchlist.h"
#include <fstream>

HtmlWatchlist::HtmlWatchlist(const string& _file) {
    this->file = _file;
}

void HtmlWatchlist::write_to_file() {
    std::ofstream f(this->file);
    f << "<!DOCTYPE "
         "html>\n<html>\n\t<head>\n\t\t<title>Watchlist</title>\n\t</head>\n\t<body>\n\t\t<table "
         "border=\"1\">\n\t\t<tr>\n\t\t\t<td>Title</td>\n\t\t\t<td>Genre</td>\n\t\t\t<td>Year</"
         "td>\n\t\t\t<td>Likes</td>\n\t\t\t<td>imdb link</td>\n\t\t</tr>";
    for (Movie m : this->movies)
        f << m.to_html_tr();
    f << "\n\t\t</table>\n\t</body>\n</html>";
}
