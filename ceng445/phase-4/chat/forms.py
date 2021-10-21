from django import forms
from django.contrib.auth.models import User

BOARD_CHOICES= [
    ('1', 'Board 1'),
    ('2', 'Board 2'),
    ('3', 'Board 3'),
    ('4', 'Board 4'),
    ]

SHAPE_CHOICES = [
    ('1', 'Bowling Ball'),
    ('2', 'Marble Ball'),
    ('3', 'Tennis Ball'),
    ('4', 'Book Block'),
    ('5', 'Domino Block'),
    ('6', 'Segment'),
    ('7', 'Rotating Segment'),
    ]

class RegisterForm(forms.ModelForm):
    first_name = forms.CharField(widget=forms.TextInput(attrs={'placeholder': 'First Name'}), label='', required=True)
    last_name = forms.CharField(widget=forms.TextInput(attrs={'placeholder': 'Last Name'}), label='', required=True)

    username = forms.CharField(widget=forms.TextInput(attrs={'placeholder': 'UserName'}), label='', required=True)
    email = forms.EmailField(widget=forms.TextInput(attrs={'placeholder': 'Email Address'}), label='', required=True)
    password = forms.CharField(widget=forms.PasswordInput(attrs={'placeholder': 'Password'}), label='', required=True)

    class Meta:
        model = User
        fields = ('first_name', 'last_name', 'username', 'email', 'password')

class LoginForm(forms.Form):
    username = forms.CharField(widget=forms.TextInput(attrs={'placeholder': 'UserName'}), label='', required=True)
    password = forms.CharField(widget=forms.PasswordInput(attrs={'placeholder': 'Password'}), label='', required=True)

class BoardForm(forms.Form):
    selected_board = forms.CharField(label='What is your board choice?', widget=forms.Select(choices=BOARD_CHOICES))
    #print(selected_board)

class ShapeForm(forms.Form):
    selected_shape = forms.CharField(widget=forms.RadioSelect(choices=SHAPE_CHOICES), required=False)
    x = forms.CharField(widget=forms.NumberInput(attrs={'placeholder': 'x coordinate'}), required=False)
    y = forms.CharField(widget=forms.NumberInput(attrs={'placeholder': 'y coordinate'}), required=False)
    shape_id_1 = forms.IntegerField(max_value = 1000, required=False)
    shape_id_2 = forms.IntegerField(max_value = 1000, required=False)