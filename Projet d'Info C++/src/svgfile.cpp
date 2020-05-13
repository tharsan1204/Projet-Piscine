#include "svgfile.h"
#include "color.h"
#include "util.h"
#include <math.h>
#include <algorithm>

#define LONG_MAX 2147483647.0

const std::string svgHeader =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" ";

const std::string svgEnding = "\n\n</svg>\n";

/// Effets "Boule en relief", voir données à la fin de ce fichier
extern const std::string svgBallGradients;

std::set<std::string> Svgfile::s_openfiles;

bool Svgfile::s_verbose = true;

Svgfile::Svgfile(std::string _filename, int _width, int _height) :
    m_filename{_filename}, m_width{_width}, m_height{_height}
{

    if (s_verbose)
        std::cout << "Opening SVG output file : "
                  << m_filename << std::endl;

    if ( s_openfiles.count(m_filename) )
        throw std::runtime_error( "File " + m_filename + " already open !" );

    m_ostrm.open(m_filename);
    s_openfiles.insert(m_filename);

    if (!m_ostrm)
    {
        std::cout << "Problem opening " << m_filename << std::endl;
        throw std::runtime_error("Could not open file " + m_filename );
    }

    if (s_verbose)
        std::cout << "OK" << std::endl;

    // Writing the header into the SVG file
    m_ostrm << svgHeader;
    m_ostrm << "width=\"" << m_width << "\" height=\"" << m_height << "\">\n\n";
}

Svgfile::~Svgfile()
{
    // Writing the ending into the SVG file
    m_ostrm << svgEnding;

    // Removing this file from the list of open files
    s_openfiles.erase(m_filename);

    // No need to explicitly close the ofstream object (automatic upon destruction)
}

// Helper templated function
template<typename T>
std::string attrib(std::string name, T val)
{
    std::ostringstream oss;
    oss << name << "=\"" << val << "\" ";
    return oss.str();
}

void Svgfile::addDisk(double x, double y, double r, std::string color)
{
    m_ostrm << "<circle "
            << attrib("cx", x)
            << attrib("cy", y)
            << attrib("r" , r)
            << attrib("fill", color )
            << "/>\n";
}

//void Svgfile::addRect2(double x, double y, double hauteur, double largeur, std::string couleur)
//{
//    m_ostrm << "<rectangle "
//            << attrib("cx", x)
//            << attrib("cy", y)
//            << attrib("hauteur" , hauteur)
//            << attrib("largeur", color )
//            << "/>\n";
//}

void Svgfile::addRectangle(int x1,int y1,int width,int height, std::string color)
{
          m_ostrm << "<polygon points=\" "
            << x1 << "," << y1 << " "
            << x1+width << "," << y1 << " "
            << x1+width << "," << y1+height << " "
            << x1 << "," << y1+height << " "
            << x1 << "," << y1
            << "\" style=\"fill:" << color
            << "\" />\n";
}

void Svgfile::addRect(double x, double y, double hauteur, double largeur, std::string couleur)
{
  for (double i=0; i<y; ++i)
    {
        addLine(x, i, x+largeur, i, couleur);
    }
}

void Svgfile::addCircle(double x, double y, double r, double ep, std::string color)
{
    m_ostrm << "<circle "
            << attrib("cx", x)
            << attrib("cy", y)
            << attrib("r" , r)
            << attrib("fill", "none")
            << attrib("stroke", color )
            << attrib("stroke-width", ep )
            << "/>\n";
}

/// <polygon points="200,10 250,190 160,210" style="fill:lime;stroke:purple;stroke-width:1" />
void Svgfile::addTriangle(double x1, double y1, double x2, double y2,
                          double x3, double y3, std::string colorFill,
                          double thickness, std::string colorStroke)
{

    m_ostrm << "<polygon points=\" "
            << x1 << "," << y1 << " "
            << x2 << "," << y2 << " "
            << x3 << "," << y3
            << "\" style=\"fill:" << colorFill
            << ";stroke:" << colorStroke
            << ";stroke-width:" << thickness
            << "\" />\n";
}

void Svgfile::addTriangle(double x1, double y1, double x2, double y2, double x3, double y3, std::string colorFill)
{
    m_ostrm << "<polygon points=\" "
            << x1 << "," << y1 << " "
            << x2 << "," << y2 << " "
            << x3 << "," << y3
            << "\" style=\"fill:" << colorFill
            << "\" />\n";
}

void Svgfile::addLine(double x1, double y1, double x2, double y2, std::string color)
{
    m_ostrm << "<line "
            << attrib("x1", x1)
            << attrib("y1", y1)
            << attrib("x2", x2)
            << attrib("y2", y2)
            << attrib("stroke", color)
            << "/>\n";
}

void Svgfile::addCross(double x, double y, double span, std::string color)
{
    addLine(x-span, y-span, x+span, y+span, color);
    addLine(x-span, y+span, x+span, y-span, color);
}

void Svgfile::addText(double x, double y, std::string text, std::string color)
{
    /// <text x="180" y="60">Un texte</text>
    m_ostrm << "<text "
            << attrib("x", x)
            << attrib("y", y)
            << attrib("fill", color)
            << ">" << text << "</text>\n";
}

void Svgfile::addText(double x, double y, double val, std::string color)
{
    std::ostringstream oss;
    oss << val;
    addText(x, y, oss.str(), color);
}

void Svgfile::addGrid(double span, bool numbering, std::string color)
{
    double y=0;
    while (y<=m_height)
    {
        addLine(0, y, m_width, y, color);
        if (numbering)
            addText(5, y-5, y, color);
        y+=span;
    }

    double x=0;
    while (x<=m_width)
    {
        addLine(x, 0, x, m_height, color);
        if (numbering)
            addText(x+5, 15, x, color);
        x+=span;
    }
}

void Svgfile::draw_graph(graph g, std::string filename, const std::vector<double>& ctr){
    double max_x=0.0, max_y=0.0, min_x=LONG_MAX, min_y=LONG_MAX, tmp_x=0.0, tmp_y=0.0, offset, width, height,
     max=0.0, min=LONG_MAX, d;
    std::string name;
    auto v = g.get_vertices();
    auto ed = g.get_edges();

    // On recupère les maximas de centralité
    for(auto e: ctr){
        max = std::max(max, e);
        min = std::min(min, e);
    }

    // On récupère les maximas de coordonnée
    for(auto e: v){
        tmp_x=(double)e.get_x();
        tmp_y=(double)e.get_y();
        max_x = std::max(max_x, tmp_x);
        max_y = std::max(max_y, tmp_y);
        min_x = std::min(min_x, tmp_x);
        min_y = std::min(min_y, tmp_y);
    }
    width = max_x*150;
    height = max_y*150;
    offset = (30.0/100.0)*sqrt(width*height);

    Svgfile file(filename, width+2*offset, height+2*offset);

    // On dessine les arretes
    for(auto e: ed)
        file.addLine(util::normalize_data(v[e.first].get_x(), max_x, min_x)*width+offset,
                     util::normalize_data(v[e.first].get_y(), max_y, min_y)*height+offset,
                     util::normalize_data(v[e.second].get_x(), max_x, min_x)*width+offset,
                     util::normalize_data(v[e.second].get_y(), max_y, min_y)*height+offset,
                     color::makeRGB(0, 0, 0));
    // On dessine les sommets et leurs noms.
    for(unsigned int i=0; i<v.size(); ++i){
        if(ctr.empty()){
            name=v[i].get_name();
            d=0.0;
        }
        else{
            name=v[i].get_name()+"("+std::to_string(ctr[i])+")";
            d=util::normalize_data(ctr[i], max, min);
        }
        tmp_x = util::normalize_data(v[i].get_x(), max_x, min_x)*width;
        tmp_y =  util::normalize_data(v[i].get_y(), max_y, min_y)*height;
        file.addDisk(tmp_x+offset, tmp_y+offset, width/100, color::get_hue_rgb(d));
        file.addText(tmp_x+offset, tmp_y+(90/100.0)*offset, name, color::makeRGB(0, 0, 0));
    }
}

