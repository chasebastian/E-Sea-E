#Pyramid Imports
from wsgiref.simple_server import make_server
from pyramid.config import Configurator
from pyramid.response import Response, FileResponse

#Bluetooth import
from Communication import Communication


# TODO: Need to add imports for bluetooth
# TODO: Create bluetooth communication object

Ser = Communication("COM6", 9600)

def get_home(req):
    return FileResponse("index.html")


#Reads current temperature data from Arduino, returns it to website to display
def update(req):

    #Insert code for reading current temp (in F) via bluetooth connection
    currentTemperature = 0
    message = Ser.receive_message(50)

    if message is not None:
        print(message) 
        try: 
            currentTemperature = float(message)
        except: 
            print(repr(message))
        

    return {'currentTemperature' : currentTemperature } 

def set_parameters(req):

    temperature = req.matchdict['temperature']
    feed = req.matchdict['feed']
    on_time = req.matchdict['on_hour'] + ":" + req.matchdict['on_minute']
    off_time = req.matchdict['off_hour'] + ":" + req.matchdict['off_minute']

    message = "/" + temperature + "/" + feed + "/" + on_time + "/" + off_time + "/"

    Ser.send_message(message)

    return {} #return empty JSON object to prevent Pyramid errors. This function does not need to respond, just send info to arduino





if __name__ == '__main__':
    with Configurator() as config:

        config.add_route('home', '/')
        config.add_view(get_home, route_name='home')

        config.add_route("set_parameters", '/Temperature/{temperature}/Feed/{feed}/on/{on_hour}:{on_minute}/off/{off_hour}:{off_minute}')
        config.add_view(set_parameters, route_name="set_parameters", renderer='json')

        config.add_route("update", "/update")
        config.add_view(update, route_name="update", renderer='json')

        

        config.add_static_view(name='/', path='./src', cache_max_age=3600)

        app = config.make_wsgi_app()
 
server = make_server('0.0.0.0', 6543, app)
print('Web server started on: http://0.0.0.0:6543')
server.serve_forever()


