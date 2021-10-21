from django.conf.urls import include
from django.urls import path
from django.contrib import admin

from django.conf.urls import url

from chat import views

urlpatterns = [
    path('chat/', include('chat.urls')),
    path('admin/', admin.site.urls),
    
    url(r'^$', views.HomeView.as_view(), name='home'),
    path('home/', views.HomeView.as_view(), name='home'),
    path('register/', views.RegisterView.as_view(), name='register'),
    path('login/', views.LoginView.as_view(), name='login'),
    path('logout/', views.LogoutView.as_view(), name='logout'),
    url(r'^board/(?P<board_id>\d+)$', views.BoardView.as_view(), name='board'),
]
