#include "repositoryhtml.h"
#include <fstream>

MovieRepoHtml::MovieRepoHtml(const string& _file) : MovieRepo(_file) {
    this->file_html = this->file;
    this->file_html.replace(this->file.find('.'), 4, ".html");
}

void MovieRepoHtml::write_to_file() {
    MovieRepo::write_to_file();
    std::ofstream f(this->file_html);
    f << "<!DOCTYPE "
         "html>\n<html>\n\t<head>\n\t\t<title>Watchlist</title>\n\t</head>\n\t<body>\n\t\t<table "
         "border=\"1\">\n\t\t<tr>\n\t\t\t<td>Title</td>\n\t\t\t<td>Genre</td>\n\t\t\t<td>Year</"
         "td>\n\t\t\t<td>Likes</td>\n\t\t\t<td>imdb link</td>\n\t\t</tr>";
    for (Movie m : this->movies)
        f << m.to_html_tr();
    f << "\n\t\t</table>\n\t</body>\n</html>";
}

void MovieRepoHtml::open() {
    this->write_to_file();
    system(("xdg-open " + this->file_html).c_str());
}
