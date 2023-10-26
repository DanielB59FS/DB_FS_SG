//vector<Vec2f> a;
//unsigned int b = 6, c = 0;
//float e = 0, f, l;
//Vec2f g;
//MovingObject h;
//PropulsionFlyweight k;


void Behavior(float d)
{
	if(c >= a.size())
		c = 0;
	if(e < 0)
	{
		Vec2f i = g - h->GetPosition();
		i.Normalize();
		l = SaiphApp::GetHeadingFromUnitVector(i);
		h->SetHeading(l);

		Vec2f j = a[c] - h->GetPosition();
		float f = h->GetWidth() / 2;
		if(j * j < f * f)
		{
			e = 1.0f;
			c++;
		}
		j.Normalize();
		k = SaiphApp::GetPropulsionFlyweight(b);
		j *= k.force;
		h->SetVelocity(j);
	}
	else
	{
		e -= d;
		h->SetHeading(RandomFloat(0, 2 * PI).GenerateValue());
	}

	h->Heartbeat(d);

}