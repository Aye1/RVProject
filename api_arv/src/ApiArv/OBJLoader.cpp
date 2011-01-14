#include "OBJLoader.h"

using namespace arv;

ObjLoader::ObjLoader(char * objfile) {
    init();
    f.open(objfile);
    if (!f.is_open()) {
        std::cout << "Unable to open the file " << objfile << std::endl;
        return;
    }
    path = std::string(objfile);
    int idx = path.rfind("/");
    if (idx == std::string::npos) path = "./";
    else path = path.substr(0,idx+1);
    loaded = load();

    if (!loaded) {
        std::cout << "Error while loading the file" << std::endl;
        return;
    }

    // Reallocation in array
    alloc();
    vertices.clear();
    normals.clear();
    texCoords.clear();
    faces.clear();

    loaded = true;
}

void ObjLoader::alloc() {
    verticesA = new Vertex[vertices.size() + 1];
    normalsA = new Normal[normals.size() + 1];
    texCoordsA = new TexCoord[texCoords.size() + 1];
    facesA = new Triangle[faces.size() + 1];

    for (unsigned int i = 0; i < vertices.size(); i++) verticesA[i + 1] = vertices[i];
    for (unsigned int i = 0; i < normals.size(); i++) normalsA[i + 1] = normals[i];
    for (unsigned int i = 0; i < texCoords.size(); i++) texCoordsA[i + 1] = texCoords[i];
    for (unsigned int i = 0; i < faces.size(); i++) {
        for (int j = 0; j < 3; j++) facesA[i + 1].vertex[j] = faces[i].vertex[j];
        for (int j = 0; j < 3; j++) facesA[i + 1].normal[j] = faces[i].normal[j];
        for (int j = 0; j < 3; j++) facesA[i + 1].texCoord[j] = faces[i].texCoord[j];
    }
    nbFaces = faces.size() + 1;
    nbVertices = vertices.size() + 1;
    nbNormals = normals.size() + 1;
    nbTexCoords = texCoords.size() + 1;

}

ObjLoader::~ObjLoader() {
    if (f.is_open()){
        f.close();
    }
}


void ObjLoader::init(){
    loaded = false;
    vertices.clear();
    normals.clear();
    texCoords.clear();
    faces.clear();
    hasmtllib = false;
    verticesA = NULL;
    normalsA = NULL;
    texCoordsA = NULL;
    facesA = NULL;
}

bool ObjLoader::load() {
    if (!f.is_open()) return false;
    std::string s;
    char c;
    int i = 0;
    int count = 0;
    while(!f.eof()) {
        getline(f,s);
        count++;
        i = 0;
        while (i < (int)s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')) i++;
        if (i == (int)s.size()){
            //std::cout << count << " : Empty line " << std::endl;
            continue;
        }
        c = s[i];

        if (c == '#'){
            // comments
        }else if (c == 'g'){
            // group name
            std::cout << count << " : global line " << s << std::endl;
        } else if (c == 'o') {
            // object name
            std::cout << count << " : object name " << s << std::endl;
        } else if (c == 'v') {
            c = s[i+1];
            if (c == 't') {
                // Texture coordinates
                if (!extractTexCoord(s)){
                    std::cout << "Error while extracting texture coordinates for " << s << std::endl;
                    return false;
                }
            }else if (c == 'n'){
                // Normal
                if (!extractNormal(s)){
                    std::cout << "Error while extracting Normal for " << s << std::endl;
                    return false;
                }

            }else if (c == ' ' || c == '\t') {
                // Vertex
                if (!extractVertex(s)){
                    std::cout << "Error while extracting Vertex for " << s << std::endl;
                    return false;
                }
            }else {
                std::cout << "Unknown line : " << s << std::endl;
            }
        } else if (c == 'f') {
            // Face
            if (!extractFace(s)){
                std::cout << count << " : Error while extracting Face for " << s << std::endl;
                return false;
            }
        } else if (c == 'm') {
            // Check for mtllib
            if (!extractmtllib(s)){
                std::cout << "Error while extracting mtllib for " << s << std::endl;
                return false;
            }
        } else if (c == 'u') {
            // Check for usemtl
            if (!extractusemtl(s)){
                std::cout << "Error while extracting usemtl for " << s << std::endl;
                return false;
            }
        } else if (c == 's') {
            // smooth shading
        }
    }

    return true;
}

bool ObjLoader::extractTexCoord(std::string s){
    std::istringstream iss(s);
    std::string param;
    TexCoord t;
    iss >> std::ws;
    // Should be v
    iss >> param;
    if (param != "vt"){
        std::cout << "Invalid parameter, expected 'vt' but received '" << param << "'" << std::endl;
        return false;
    }
    iss >> std::ws;
    iss >> t.u;
    if (iss.bad() || iss.eof()) {
        std::cout << "Error while parsing the value" << std::endl;
        return false;
    }
    iss >> std::ws;
    iss >> t.v;
    if (iss.bad() || iss.eof()) {
        std::cout << "Error while parsing the value" << std::endl;
        return false;
    }
    iss >> std::ws;
    if (iss.bad() || iss.eof()) {
        t.w = 0.0;
    }else{
        iss >> t.w;
    }
    
    if (!(iss.bad() || iss.eof())) {
        std::cout << "Garbage on the line : " << s << std::endl;
        return false;
    }
    texCoords.push_back(t);

    return true;
}
bool ObjLoader::extractNormal(std::string s){

    std::istringstream iss(s);
    std::string param;
    Normal n;
    iss >> std::ws;
    // Should be v
    iss >> param;
    if (param != "vn"){
        std::cout << "Invalid parameter, expected 'vn' but received '" << param << "'" << std::endl;
        return false;
    }

    iss >> std::ws;
    iss >> n.x;
    if (iss.bad() || iss.eof()) {
        std::cout << "Error while parsing the value" << std::endl;
        return false;
    }
    iss >> std::ws;
    iss >> n.y;
    if (iss.bad() || iss.eof()) {
        std::cout << "Error while parsing the value" << std::endl;
        return false;
    }
    iss >> std::ws;
    iss >> n.z;
    if (!(iss.bad() || iss.eof())) {
        std::cout << "Garbage on the line : " << s << std::endl;
        return false;
    }

    normals.push_back(n);

    return true;
}
bool ObjLoader::extractVertex(std::string s){
    std::istringstream iss(s);
    std::string param;
    Vertex v;
    iss >> std::ws;
    // Should be v
    iss >> param;
    if (param != "v"){
        std::cout << "Invalid parameter, expected 'v' but received '" << param << "'" << std::endl;
        return false;
    }
    iss >> std::ws;
    iss >> v.x;
    if (iss.bad() || iss.eof()) {
        std::cout << "Error while parsing the value" << std::endl;
        return false;
    }
    iss >> std::ws;
    iss >> v.y;
    if (iss.bad() || iss.eof()) {
        std::cout << "Error while parsing the value" << std::endl;
        return false;
    }
    iss >> std::ws;
    iss >> v.z;
    if (!(iss.bad() || iss.eof())) {
        std::cout << "Garbage on the line : " << s << std::endl;
        return false;
    }
    vertices.push_back(v);

    return true;
}
bool ObjLoader::extractFace(std::string s){
    std::istringstream iss(s);
    std::string param;
    int valV,valT,valN;
    Face f;
    char c;
    iss >> std::ws;
    iss >> param;
    if (param != "f"){
        std::cout << "Invalid parameter, expected 'f' but received '" << param << "'" << std::endl;
        return false;
    }

    while (!iss.eof()) {
        valV = -1;
        valT = -1;
        valN = -1;
        iss >> std::ws;
        iss >> valV;
        iss >> c;
        if (c != '/'){
            std::cout << "Error while parsing " << s << std::endl;
            return false;
        }
        iss >> valT;
        iss >> c;
        if (c != '/'){
            std::cout << "Error while parsing " << s << std::endl;
            return false;
        }
        iss >> valN;
        //iss >> c;
        //std::cout << "c = " << (char)c << std::endl;
        //if (c != ' ' && c != '\t'){
        //    std::cout << "Error while parsing " << s << " expected whitespace" << std::endl;
        //    return false;
        //}
        
        f.vertex.push_back(valV);
        f.normal.push_back(valN);
        f.texCoord.push_back(valT);
    }
    faces.push_back(f);
    


    return true;
}


bool ObjLoader::extractusemtl(std::string s){
    std::istringstream iss(s);
    std::string param;
    Pair p;
    iss >> std::ws;
    iss >> param;
    if (param != "usemtl"){
        std::cout << "Invalid parameter, expected 'usemtl' but received '" << param << "'" << std::endl;
        return false;
    }
    
    iss >> std::ws;
    iss >> param;
  
    unsigned int i = 0; 
    int idx = -1;
    for (i = 0; i < materials.size(); i++) {
        if (param == materials[i].name){
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        std::cout << "Unknown material : '" << param << "'" << std::endl;
        return false;
    }
    p.id_mat = idx;
    p.id_face = faces.size() + 1;
    pairs.push_back(p);
    //std::cout << "Face / Mat : " << p.id_mat << " " << p.id_face << std::endl; 
    return true;
}



bool ObjLoader::extractmtllib(std::string s){
    std::istringstream iss(s);
    std::string param;
    iss >> std::ws;
    iss >> param;
    if (param != "mtllib"){
        std::cout << "Invalid parameter, expected 'mtllib' but received '" << param << "'" << std::endl;
        return false;
    }

    iss >> std::ws;
    iss >> param;
    mtllib = path + param;


    // Try to open the file
    std::ifstream g(mtllib.c_str());
    if (!g.is_open()) {
        std::cout << "Unable to open the mtllib file " << mtllib << std::endl;
        hasmtllib = false;
        return true;
    }
    Material m;
    while (! g.eof()) {
        g >> param;
        if (param == "#") {
            continue;
        }else if (param == "newmtl"){
            if (m.name != ""){
                materials.push_back(m);
                m.name = "";
            }
            g >> m.name;
        }else if (param == "Ka"){
            g >> m.Ka[0];
            g >> m.Ka[1];
            g >> m.Ka[2];
            m.Ka[3] = 1.0;
        }else if (param == "Kd"){
            g >> m.Kd[0];
            g >> m.Kd[1];
            g >> m.Kd[2];
            m.Kd[3] = 1.0;
        }else if (param == "Ks"){
            g >> m.Ks[0];
            g >> m.Ks[1];
            g >> m.Ks[2];
            m.Ks[3] = 1.0;
        }else if (param == "d"){
            g >> m.d;
        }else if (param == "Ns"){
            g >> m.Ns;
        }else if (param == "illum"){
            g >> m.illum;
        }else if (param == "map_Kd"){
            g >> m.map_Kd;
        }

    }

    hasmtllib = true;
    // Check if file exists

    // Load Textures
    for (unsigned int i = 0; i < materials.size(); i++){
        loadTexture(materials[i]);
        //std::cout << "Loading '" << materials[i].name << "' , id = " << materials[i].texture << std::endl;
    }
    g.close();
    return true;
}

void ObjLoader::loadTexture(Material & mat){
    if (mat.map_Kd == "") {
        mat.texture = -1;
        return;
    }
    std::string file = path + mat.map_Kd;
    QImage t;
    QImage b;
    if (!b.load(file.c_str())){
        b = QImage(16,16,QImage::Format_RGB32);
        b.fill(Qt::green);
    }

    t = QGLWidget::convertToGLFormat(b);
    glGenTextures(1,&(mat.texture));
    glBindTexture(GL_TEXTURE_2D,mat.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


void ObjLoader::pushMaterial(Material const & m){
    glMaterialfv(GL_FRONT, GL_SPECULAR, m.Ks);
    glMaterialfv(GL_FRONT, GL_SHININESS, &m.Ns);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m.Kd);
    glMaterialfv(GL_FRONT, GL_AMBIENT, m.Ka);
    //std::cout << m.Ka[0] << " " << m.Ks[1] << " " << m.Ks[2] <<  " " << m.Ks[3] << std::endl;
}

void ObjLoader::render() {
    //GLboolean texEnabled = glIsEnabled( GL_TEXTURE_3D );
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,materials[0].texture);
    glBegin( GL_TRIANGLES );
    int idx = 0;
    unsigned int id_pairs = 0;
    for (unsigned int i = 1; i < nbFaces; i++) {
        if (id_pairs < pairs.size() && i == pairs[id_pairs].id_face) {
            pushMaterial(materials[pairs[id_pairs].id_mat]);
            id_pairs ++;
        }
        for (int j = 0; j < 3; j++){
            idx = facesA[i].normal[j];
            glNormal3f(normalsA[idx].x,normalsA[idx].y,normalsA[idx].z);

            idx = facesA[i].texCoord[j];
            glTexCoord2f(texCoordsA[idx].u,texCoordsA[idx].v);
            //glTexCoord3f(texCoordsA[idx].u,texCoordsA[idx].v,texCoordsA[idx].w);
            
            idx = facesA[i].vertex[j];
            glVertex3f(verticesA[idx].x,verticesA[idx].y,verticesA[idx].z);
        }

    }

    glEnd();
    //	if ( texEnabled )
    glEnable( GL_TEXTURE_2D );
    //	else
    //		glDisable( GL_TEXTURE_3D );
    //
}
