from django.db import models

from django_matplotlib.fields import MatplotlibFigureField
import json

class BoardModel(models.Model):
    bid = models.CharField(max_length=10, primary_key=True)
    bname = models.CharField(max_length=30, unique=True)
    loadFlag = models.BooleanField(default=False)
    bstate = models.CharField(max_length=1000, default='{}')

    def __str__(self):
        return self.bname

    def setState(self, state):
        #print("BEFORE DUMPING JSON")
        #print(state)
        self.bstate = json.dumps(state)

    def getState(self):
        return json.loads(self.bstate)
