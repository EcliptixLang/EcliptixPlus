#include <ENV.hpp>
#include <FunctionValues.hpp>
#include <winutils.hpp>
#include <raylib/raylib.h>

using Nodes = AST::Nodes; 
using string = std::string;

std::shared_ptr<Values::Runtime> fipos(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    if (args[0]->type() != "number" || args[1]->type() != "number"){
        DisplayErrorMessageBox("showFPS(x, y) Errored out: one of its values is not a number.");
        exit(1);
    }

    Values::Number *a = dynamic_cast<Values::Number*>(args[0].get());
    Values::Number *b = dynamic_cast<Values::Number*>(args[1].get()); 
    
    DrawFPS(a->value, b->value);

    return std::make_shared<Values::Null>();
}

std::shared_ptr<Values::Runtime> shudclose(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    return std::make_shared<Values::Boolean>(WindowShouldClose());
}
void CustomLog(int msgType, const char *text, va_list args)
{ 
  return;
}
std::shared_ptr<Values::Runtime> cretwindow(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    SetTraceLogCallback(CustomLog);
    InitWindow(800, 600, "Test");
    return std::make_shared<Values::Null>();
}

std::shared_ptr<Values::Runtime> rmwindow(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    CloseWindow();
    return std::make_shared<Values::Null>();
}

std::shared_ptr<Values::Runtime> startdrawing(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    BeginDrawing();
    ClearBackground(BLACK);
    return std::make_shared<Values::Null>();
}

std::shared_ptr<Values::Runtime> stopdrawing(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    EndDrawing();
    return std::make_shared<Values::Null>();
}

std::shared_ptr<Values::Runtime> tringol(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    if (args[0]->type() != "object" || args[1]->type() != "object" || args[1]->type() != "object" || args[1]->type() != "object"){
        DisplayErrorMessageBox("drawTriangle(o1, o2, o3, c) Errored out: one of its values is not an object.");
        exit(1);
    }

    Values::Object *a = dynamic_cast<Values::Object*>(args[0].get());
    Values::Object *b = dynamic_cast<Values::Object*>(args[1].get());
    Values::Object *c = dynamic_cast<Values::Object*>(args[2].get());
    Values::Object *d = dynamic_cast<Values::Object*>(args[3].get());

    if(a->props["x"] == nullptr || a->props["y"] == nullptr){
        DisplayErrorMessageBox("drawTriangle(o1, o2, o3, c) Errored out: Point One of triangle is null.");
        exit(1);
    }
    if(b->props["x"] == nullptr || b->props["y"] == nullptr){
        DisplayErrorMessageBox("drawTriangle(o1, o2, o3, c) Errored out: Point Two of triangle is null.");
        exit(1);
    }
    if(c->props["x"] == nullptr || c->props["y"] == nullptr){
        DisplayErrorMessageBox("drawTriangle(o1, o2, o3, c) Errored out: Point Three of triangle is null.");
        exit(1);
    }
    if(d->props["r"] == nullptr || d->props["g"] == nullptr || d->props["b"] == nullptr){
        DisplayErrorMessageBox("drawTriangle(o1, o2, o3, c) Errored out: Color of triangle is null.");
        exit(1);
    }

    if(a->props["x"]->type() != "number" || a->props["y"]->type() != "number"){
        DisplayErrorMessageBox("drawTriangle(o1, o2, o3, c) Errored out: a point on o1 is not a number.");
        exit(1);
    }
    if(b->props["x"]->type() != "number" || b->props["y"]->type() != "number"){
        DisplayErrorMessageBox("drawTriangle(o1, o2, o3, c) Errored out: a point on o2 is not a number.");
        exit(1);
    }
    if(c->props["x"]->type() != "number" || c->props["y"]->type() != "number"){
        DisplayErrorMessageBox("drawTriangle(o1, o2, o3, c) Errored out: a point on o3 is not a number.");
        exit(1);
    }
    if(d->props["r"]->type() != "number" || d->props["g"]->type() != "number" || d->props["b"]->type() != "number"){
        DisplayErrorMessageBox("drawTriangle(o1, o2, o3, c) Errored out: a point on c is is not a number.");
        exit(1);
    }

    Values::Number *a1 = dynamic_cast<Values::Number*>(a->props["x"].get());
    Values::Number *b1 = dynamic_cast<Values::Number*>(a->props["y"].get());
    Vector2 v1 = { (float)a1->value, (float)b1->value };
    Values::Number *a2 = dynamic_cast<Values::Number*>(b->props["x"].get());
    Values::Number *b2 = dynamic_cast<Values::Number*>(b->props["y"].get());
    Vector2 v2 = { (float)a2->value, (float)b2->value };

    Values::Number *a3 = dynamic_cast<Values::Number*>(c->props["x"].get());
    Values::Number *b3 = dynamic_cast<Values::Number*>(c->props["y"].get());
    Vector2 v3 = { (float)a3->value, (float)b3->value };

    Values::Number *ac = dynamic_cast<Values::Number*>(d->props["r"].get());
    Values::Number *bc = dynamic_cast<Values::Number*>(d->props["g"].get());
    Values::Number *cc = dynamic_cast<Values::Number*>(d->props["b"].get());
    Color c1 = { (unsigned char)ac->value, (unsigned char)bc->value, (unsigned char)cc->value, 255 };
    DrawTriangle(v1, v2, v3, c1);

    return std::make_shared<Values::Null>();
}

std::shared_ptr<Values::Runtime> thing(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    std::string value = "";
    
    for(auto& arg : args){
        value.append(" ").append(arg->stringValue());
    }

    if(value[0] == ' '){
        value.erase(0, 1);
    }

    std::cout << value << "\n";

    return std::make_shared<Values::Null>(Values::Null());
}

std::shared_ptr<Values::Runtime> thingn(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    std::string value = "";
    
    for(auto& arg : args){
        value.append(" ").append(arg->stringValue());
    }

    if(value[0] == ' '){
        value.erase(0, 1);
    }

    std::cout << value;

    return std::make_shared<Values::Null>(Values::Null());
}

std::shared_ptr<Values::Runtime> ask(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    thingn(std::move(args), env);
    std::string ans;
    std::cin >> ans;
    
    return std::make_shared<Values::String>(Values::String(ans));
}

std::shared_ptr<Values::Runtime> _throw(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    std::cout << "Error:\n- Code: 8\n- Description: ";
    thing(std::move(args), env);
    exit(8);

    return std::make_shared<Values::Null>(Values::Null());
}

std::shared_ptr<Values::Runtime> readFile(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    if(args[0].get() == nullptr){
        std::cout << "Cannot read null\n";
        exit(6);
    }

    std::string filecont = Utilities::readFile(args[0]->stringValue());

    return std::make_shared<Values::String>(Values::String(filecont));
}

std::shared_ptr<Values::Runtime> writeFile(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    if(args[0]->type() != "string"){
        std::cout << "Cannot read a non string\n";
        exit(6);
    }
    if(args[1]->type() != "string"){
        std::cout << "Cannot read a non string\n";
        exit(6);
    }
    
    Utilities::writeFile(args[0]->stringValue(), args[1]->stringValue());

    return std::make_shared<Values::Null>(Values::Null());
}

std::shared_ptr<Values::Runtime> holt(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    if(args[0]->type() != "number"){
        std::cout << "Cannot read a non number\n";
        exit(6);
    }

    sleep(dynamic_cast<Values::Number*>(args[0].get())->value * 1000);

    return std::make_shared<Values::Null>(Values::Null());
}

std::shared_ptr<Values::Runtime> exeet(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    if(args[0]->type() != "number"){
        std::cout << "Cannot read a non number\n";
        exit(6);
    }

    exit(dynamic_cast<Values::Number*>(args[0].get())->value);
}

std::shared_ptr<Values::Runtime> errout(std::vector<std::shared_ptr<Values::Runtime>> args, Environment& env){
    std::string value = "";
    
    for(auto& arg : args){
        value.append(" ").append(arg->stringValue());
    }

    if(value[0] == ' '){
        value.erase(0, 1);
    }

    std::cerr << value << "\n";

    return std::make_shared<Values::Null>(Values::Null());
}

std::map<std::string, std::shared_ptr<Values::Runtime>> ErrorStuff;
std::map<std::string, std::shared_ptr<Values::Runtime>> ConsoleStuff;
std::map<std::string, std::shared_ptr<Values::Runtime>> FileStuff;
std::map<std::string, std::shared_ptr<Values::Runtime>> ProcessStuff;
std::map<std::string, std::shared_ptr<Values::Runtime>> ProjectStuff;
std::map<std::string, std::shared_ptr<Values::Runtime>> AppStuff;

void Environment::setup(){
    ErrorStuff["throw"] = std::make_shared<NativeFN>(NativeFN(_throw));
    ErrorStuff["out"] = std::make_shared<NativeFN>(NativeFN(errout));
	ConsoleStuff["out"] = std::make_shared<NativeFN>(NativeFN(thing));
    ConsoleStuff["ask"] = std::make_shared<NativeFN>(NativeFN(ask));
	FileStuff["read"] = std::make_shared<NativeFN>(NativeFN(readFile));
    FileStuff["write"] = std::make_shared<NativeFN>(NativeFN(writeFile));
    ProcessStuff["exit"] = std::make_shared<NativeFN>(NativeFN(exeet));
    ProcessStuff["wait"] = std::make_shared<NativeFN>(NativeFN(holt));
    AppStuff["showFPS"] = std::make_shared<NativeFN>(fipos);
    AppStuff["drawTriangle"] = std::make_shared<NativeFN>(tringol);
    AppStuff["createWindow"] = std::make_shared<NativeFN>(cretwindow);
    AppStuff["deleteWindow"] = std::make_shared<NativeFN>(rmwindow);
    AppStuff["shouldClose"] = std::make_shared<NativeFN>(shudclose);
    AppStuff["startDrawing"] = std::make_shared<NativeFN>(startdrawing);
    AppStuff["endDrawing"] = std::make_shared<NativeFN>(stopdrawing);

	this->setVariableSafe("error", std::make_shared<Values::Object>(Values::Object(ErrorStuff)), true);
	this->setVariableSafe("console", std::make_shared<Values::Object>(Values::Object(ConsoleStuff)), true);
	this->setVariableSafe("file", std::make_shared<Values::Object>(Values::Object(FileStuff)), true);
    this->setVariableSafe("process", std::make_shared<Values::Object>(Values::Object(ProcessStuff)), true);
    this->setVariableSafe("app", std::make_shared<Values::Object>(AppStuff), true);
	this->setVariableSafe("null", std::make_shared<Values::Null>(Values::Null()), true);
	this->setVariableSafe("true", std::make_shared<Values::Boolean>(Values::Boolean(true)), true);
	this->setVariableSafe("false", std::make_shared<Values::Boolean>(Values::Boolean(false)), true);
}