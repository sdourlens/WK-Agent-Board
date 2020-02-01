
WK Carte Agent => application Android pour faire écran de la carte agent

Mobile.TextScreen.Set(parameter,value); // forecolor, backcolor
Mobile.TextScreen.Print("Bonjour le monde");
Mobile.TextScreen.Println("Bonjour le monde");

Mobile.GraphScreen.Pixel(color,x,y);
Mobile.GraphScreen.Line(color,x1,y1,x2,y2);
Mobile.GraphScreen.Triangle(color,x1,y1,x2,y2,x3,y3);
Mobile.GraphScreen.Circle(color,x,y,radius);
Mobile.GraphScreen.Image(filepath,x1,y1,x2,y2); // image is in the phone
Mobile.GraphScreen.Video(filepath,x,y,rate); // Video is in the phone

Mobile.WebScreen.URL(url); // web page file or url 

Mobile.Mouse.Get()
Mobile.Mouse.Set()
Mobile.Mouse.Click()

Mobile.Keyboard.Get()
Mobile.Keyboard.Set()

Mobile.Gesture.Get()


Mobile.Disk.Set(parameter,value);
Mobile.Disk.Copy(source,target);
Mobile.Disk.Delete(file);
Mobile.Disk.Move(source,target);
Mobile.Disk.OpenFile(file);
Mobile.Disk.ReadFile(text);
Mobile.Disk.WriteFile(text);

Mobile.Data.Set(parameter,value);
Mobile.Data.Query(request);

Mobile.KB.Set(parameter,value);
Mobile.KB.Query(request);

Mobile.Speech.Set(parameter,value); // language, voice, ...
Mobile.Speech.Say(sentence);
Mobile.Speech.Listen(sentence);

Mobile.App.Install(app);
Mobile.App.Open(app,args); // app or intent
Mobile.App.Close(app);
Mobile.App.Uninstall(app);

Mobile.Mail.Set()
Mobile.Mail.Query()

Mobile.Agenda.Set()
Mobile.Agenda.Query()

