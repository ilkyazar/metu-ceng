import pymunk

class Connector():
    def __init__(self, body1, body2):
        self.body1 = body1
        self.body2 = body2
        self.setPinJoint()

    def setPinJoint(self):
        self.pin_joint = pymunk.PinJoint(self.body1, self.body2, (20, 0), (-20, 0))

    def getPinJoint(self):
        return self.pin_joint