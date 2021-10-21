virtual environment:

        virtualenv -p python3.6 script_env //create  
        source script_env/bin/activate //activate  


start redis server:
    redis-server --port 6380


start web server: 
	
	python manage.py runserver